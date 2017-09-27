NGUYEN Duc Hau (NGUD03039508)
VANLEENE Sacha (VANS27129409)
FAGET Thomas   (FAGT03089508)

Fonctionnalités : 

-Refactoring : Reformation de l'architecture du projet : Réagencement pour éviter que le dossier common soit en double

-Ajout d'une 3ème entité : Boozer , cette entité reste au saloon passe son temps à boire et attend que le miner vienne l'interrompre pour se battre. Le mineur et le boozer rentrent en phase de combat et communiquent via des messages, ils s'informent lorsqu'ils réussissent leurs coups au bout de 5 coups touchés l'une des deux entités devient KO et prévient l'autre de la fin du combat. La phase de KO dure également 5 temps.

-Version MultiThread : Les 3 entités sont lancées sur des threads qui tournent en continu. Ils sont donc executés en même temps (en parallèle), il en va de même pour le dispatcher qui est lancé sur un autre thread. Pour réaliser cela nous avons identifiés les ressources partagées tel que : la console et le dispatcher. Nous avons donc sécurisé l'accés à ces ressources en utilisant des mutex ce qui implique la création d'une structure de communication conçu spécialement pour le multi threading.