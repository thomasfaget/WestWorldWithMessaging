NGUYEN Duc Hau (NGUD03039508)
VANLEENE Sacha (VANS27129409)
FAGET Thomas   (FAGT03089508)

Fonctionnalit�s : 

-Refactoring : Reformation de l'architecture du projet : R�agencement pour �viter que le dossier common soit en double

-Ajout d'une 3�me entit� : Boozer , cette entit� reste au saloon passe son temps � boire et attend que le miner vienne l'interrompre pour se battre. Le mineur et le boozer rentrent en phase de combat et communiquent via des messages, ils s'informent lorsqu'ils r�ussissent leurs coups au bout de 5 coups touch�s l'une des deux entit�s devient KO et pr�vient l'autre de la fin du combat. La phase de KO dure �galement 5 temps.

-Version MultiThread : Les 3 entit�s sont lanc�es sur des threads qui tournent en continu. Ils sont donc execut�s en m�me temps (en parall�le), il en va de m�me pour le dispatcher qui est lanc� sur un autre thread. Pour r�aliser cela nous avons identifi�s les ressources partag�es tel que : la console et le dispatcher. Nous avons donc s�curis� l'acc�s � ces ressources en utilisant des mutex ce qui implique la cr�ation d'une structure de communication con�u sp�cialement pour le multi threading.