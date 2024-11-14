Lancement du pathfind :
Pour lancer la recherche de chemin il suffit de faire click droit lorsque le jeu run.

Placement des point de départ et d'arrivé :
La position de ces deux point sont respectivement renseigné dans les variables StartingPoint et FinishPoint définient dans PathGrid.h, la valeur x ne doit pas excéder la variable Height et la valeur Y 
Width.

Placement des obstacles :
Le placement des obstacles se fait via les tableaux :	
	TArray<double> BlockX;
	TArray<double> BlockY;
un obstacle est présent sur la carte si BlockX[coordoneéX] > 1 et BlockY[coordoneéY] > 1, ces coordoneés peuvent êtres définies à la main au début du BeginPlay de APathGrid