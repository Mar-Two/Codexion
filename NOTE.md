## Conception scheduler — décidée le [date]

### Verrous
- UN SEUL mutex pour tout l'état d'allocation : data->dongle_mutex
  → protège : dongles[].taken_by, dongles[].available_at, dongles[].queue
- Suppression des mutex individuels par dongle
- Conséquence : plus de tri par indice, le deadlock sur ordre de verrous disparaît
- coder_mutex : protège nb_compiles et last_compile_start (lus par le moniteur)
- print_mutex : protège stdout

### Condvar
- UNE condvar globale dans data, associée à dongle_mutex
- Réveil collectif (broadcast), jamais signal : le noyau choisirait au hasard

### Structure de la boucle d'acquisition
verrouiller dongle_mutex
  calculer la clé (fifo → timestamp | edf → last_compile_start + time_to_burnout)
  s'inscrire dans les DEUX files          ← UNE SEULE FOIS, avant la boucle
  while (prédicat faux)
      cond_wait ou cond_timedwait
  extraire des deux files
  marquer les deux dongles pris
déverrouiller

→ logs, sleep compilation, libération, debug, refactor : TOUT HORS VERROU

### Prédicat (3 parties + arrêt)
1. je suis en tête de mes DEUX files (peek + comparaison d'id)
2. les deux dongles ont taken_by == -1
3. now >= available_at pour les deux
(+ plus tard : drapeau d'arrêt)

### Libération
verrouiller → taken_by = -1, available_at = now + cooldown → broadcast → déverrouiller

### Points clés
- TOUJOURS while, jamais if, autour de l'attente (réveils spurieux + broadcast)
- Inscription atomique dans les deux files → garantit qu'un coder au moins
  est en tête de ses deux files (celui de plus petite clé). Pas de blocage vivant.
- Le temps ne se signale pas → cond_timedwait pour le cooldown,
  avec available_at comme date ABSOLUE (pas une durée)
- Une expiration de timedwait ne prouve rien → retour dans le while

### Ordre de codage (si le temps manque, s'arrêter après l'étape 2)
1. Refonte des verrous (mécanique, ~2h)
2. Inscription + prédicat SANS cooldown → vérifier : plus de famine, helgrind propre
3. Cooldown avec timedwait