#Mise à zéro d'un vecteur

##En fonction de la taille du vecteur

###N=1000
Evaluation : N=1000, type=float
ZERO	7.68706
ZERO	2.57014
ZERO	2.47794
ZERO	2.46685
ZERO	2.4805
ZERO	2.48305
ZERO	2.46335
ZERO	2.40034
ZERO	2.38195
ZERO	2.39053

Evaluation : N=1000, type=int
ZERO	7.32429
ZERO	2.47117
ZERO	2.33191
ZERO	2.34047
ZERO	2.34003
ZERO	2.33227
ZERO	2.34732
ZERO	2.34068
ZERO	2.35994
ZERO	2.25084

###N=10000
Evaluation : N=10000, type=float
ZERO	5.1966
ZERO	1.52408
ZERO	1.48442
ZERO	1.47884
ZERO	1.48083
ZERO	1.49095
ZERO	1.49259
ZERO	1.47988
ZERO	1.48125
ZERO	1.48449

###N=10
Evaluation : N=10, type=float
ZERO	5.1
ZERO	3.14
ZERO	3.32
ZERO	3.3
ZERO	3.4
ZERO	3.48
ZERO	3.4
ZERO	3.48
ZERO	3.3
ZERO	3.58


Pour chaque appel à la boucle de mise à zéro, on donne le temps moyen par itération (en nombre de cycles).
Sachant que la fréquence du processeur en environ 1600MHz et qu'un float fait 4 bytes, on trouve comme vitesse :
Pour N=1000, 842Mo/s puis 2.6Go/s
Pour N=10000, 1.2Go/s puis 4.3Go/s
Pour N=10, 1.2Go/s puis 2.1Go/s
On retrouve des résultats similaires en utilisant des int.

Premier temps plus long : mise en cache des instructions (surtout) et des données (un peu).
-Non détermisnisme:
prédicteur de branchement
accès concurrents au bus
cache statistique

##En fonction du type de données

###Avec des int
Evaluation : N=1000, type=int
ZERO	7.32429
ZERO	2.47117
ZERO	2.33191
ZERO	2.34047
ZERO	2.34003
ZERO	2.33227
ZERO	2.34732
ZERO	2.34068
ZERO	2.35994
ZERO	2.25084

###Avec des float
Evaluation : N=1000, type=float
ZERO	7.68706
ZERO	2.57014
ZERO	2.47794
ZERO	2.46685
ZERO	2.4805
ZERO	2.48305
ZERO	2.46335
ZERO	2.40034
ZERO	2.38195
ZERO	2.39053

###Avec des double
Evaluation : N=1000, type=double
ZERO	11.1892
ZERO	3.16504
ZERO	2.99126
ZERO	2.83424
ZERO	2.82567
ZERO	2.88735
ZERO	2.78788
ZERO	2.826
ZERO	2.83658
ZERO	2.88355

##Étude des differents niveaux d'optimisation

###Pas d'optimisation (O0)
On trouve comme résultat :
Evaluation : N=1000, type=float
ZERO	20.9335
ZERO	14.6035
ZERO	7.45287
ZERO	7.03741
ZERO	11.6927
ZERO	9.38172
ZERO	9.41502
ZERO	6.45643
ZERO	7.24108
ZERO	6.87814

Le corps de la boucle produite en assembleur est :
``` assembly
	movl	-4(%rbp), %eax
	cltq
	pxor	%xmm0, %xmm0
	movss	%xmm0, BF(,%rax,4)
	addl	$1, -4(%rbp)
```

Boucle naïve où l'on met des zéros les uns à la suite des autres.

###Optimisation O2

Evaluation : N=1000, type=float
ZERO	7.68706
ZERO	2.57014
ZERO	2.47794
ZERO	2.46685
ZERO	2.4805
ZERO	2.48305
ZERO	2.46335
ZERO	2.40034
ZERO	2.38195
ZERO	2.39053

``` assembly
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rdx, %xmm0
.L10:
	movsd	%xmm0, begin(%rip)
	movl	$BF, %eax
	.p2align 4,,10
	.p2align 3
.L11:
	movss	%xmm1, (%rax)
	addq	$4, %rax
	cmpq	%rax, %rbx
	jne	.L11
```

Utilisation du SIMD intel. On met les zéros vecteur après vecteur.

###Optimisation O3

Evaluation : N=1000, type=float
ZERO	6.28453
ZERO	1.61179
ZERO	1.43273
ZERO	1.27143
ZERO	1.29315
ZERO	1.25718
ZERO	1.24528
ZERO	1.2684
ZERO	1.2442
ZERO	1.26777

	call	memset

Appel direct de la fonction memset déjà optimisée.

#Copie de matrices

##En fonction de la taille

###N=10
Evaluation : N=10, type=int
COPY_ij	7.92
COPY_ij	5.78
COPY_ij	4.42
COPY_ij	4.92
COPY_ij	4.5
COPY_ij	4.52
COPY_ij	4.42
COPY_ij	4.5
COPY_ij	4.58
COPY_ij	5.36

COPY_ji	11.48
COPY_ji	7.14
COPY_ji	7.22
COPY_ji	7.22
COPY_ji	7.22
COPY_ji	7.22
COPY_ji	7.38
COPY_ji	7.22
COPY_ji	7.32
COPY_ji	7.14

###N=1000

Evaluation : N=1000, type=int
COPY_ij	11.8108
COPY_ij	4.12704
COPY_ij	4.0181
COPY_ij	3.90587
COPY_ij	3.88513
COPY_ij	3.88891
COPY_ij	3.88615
COPY_ij	3.88489
COPY_ij	3.89188
COPY_ij	3.89152

COPY_ji	89.0115
COPY_ji	79.0243
COPY_ji	71.8155
COPY_ji	65.9912
COPY_ji	61.5585
COPY_ji	57.9952
COPY_ji	55.0859
COPY_ji	52.239
COPY_ji	50.2057
COPY_ji	47.9815

###N=10000

Evaluation : N=10000, type=int
COPY_ij	5.32853
COPY_ij	1.90908
COPY_ij	1.89404
COPY_ij	1.89376
COPY_ij	1.89719
COPY_ij	1.89967
COPY_ij	1.89391
COPY_ij	1.89328
COPY_ij	1.95127
COPY_ij	1.91355

COPY_ji	71.7033
COPY_ji	71.8483
COPY_ji	71.6444
COPY_ji	71.9017
COPY_ji	71.7833
COPY_ji	71.8588
COPY_ji	71.7957
COPY_ji	71.8511
COPY_ji	71.954
COPY_ji	71.6344

##En fonction du type

###Avec des int
Evaluation : N=1000, type=int
COPY_ij	11.8108
COPY_ij	4.12704
COPY_ij	4.0181
COPY_ij	3.90587
COPY_ij	3.88513
COPY_ij	3.88891
COPY_ij	3.88615
COPY_ij	3.88489
COPY_ij	3.89188
COPY_ij	3.89152

COPY_ji	89.0115
COPY_ji	79.0243
COPY_ji	71.8155
COPY_ji	65.9912
COPY_ji	61.5585
COPY_ji	57.9952
COPY_ji	55.0859
COPY_ji	52.239
COPY_ji	50.2057
COPY_ji	47.9815

###Avec des float

Evaluation : N=1000, type=float
COPY_ij	12.476
COPY_ij	4.39879
COPY_ij	4.32611
COPY_ij	4.14214
COPY_ij	4.1538
COPY_ij	4.18213
COPY_ij	4.17632
COPY_ij	4.17493
COPY_ij	4.13372
COPY_ij	4.13133

COPY_ji	100.151
COPY_ji	83.7843
COPY_ji	75.2825
COPY_ji	68.9291
COPY_ji	63.9149
COPY_ji	59.9198
COPY_ji	56.7048
COPY_ji	53.7525
COPY_ji	51.2895
COPY_ji	49.1841

###Avec des double

Evaluation : N=1000, type=double
COPY_ij	16.3666
COPY_ij	3.74818
COPY_ij	3.68807
COPY_ij	3.54384
COPY_ij	3.53097
COPY_ij	3.5147
COPY_ij	3.53184
COPY_ij	3.52781
COPY_ij	3.53002
COPY_ij	3.53049

COPY_ji	82.7315
COPY_ji	74.8231
COPY_ji	69.0559
COPY_ji	64.3271
COPY_ji	60.5311
COPY_ji	57.2453
COPY_ji	54.4435
COPY_ji	52.2001
COPY_ji	49.8971
COPY_ji	48.9144

##En fonction de l'optimisation

###Pas d'optimisation (O0)

Evaluation : N=1000, type=float
COPY_ij	26.0628
COPY_ij	17.1634
COPY_ij	8.17356
COPY_ij	7.80091
COPY_ij	7.59962
COPY_ij	7.61052
COPY_ij	8.11724
COPY_ij	7.8617
COPY_ij	7.98318
COPY_ij	7.6583

COPY_ji	42.0696
COPY_ji	41.5248
COPY_ji	41.5129
COPY_ji	41.8216
COPY_ji	41.8723
COPY_ji	41.5353
COPY_ji	41.5811
COPY_ji	41.5243
COPY_ji	41.5952
COPY_ji	41.5207

###Optimisation (O2)

Evaluation : N=1000, type=float
COPY_ij	7.36525
COPY_ij	2.01488
COPY_ij	1.91573
COPY_ij	1.93252
COPY_ij	1.95345
COPY_ij	2.02385
COPY_ij	2.07696
COPY_ij	1.89563
COPY_ij	1.90043
COPY_ij	1.88921

COPY_ji	46.5381
COPY_ji	46.8177
COPY_ji	47.5235
COPY_ji	46.1705
COPY_ji	45.9089
COPY_ji	46.5044
COPY_ji	45.924
COPY_ji	46.0128
COPY_ji	45.9122
COPY_ji	46.036

###Optimisation (O3)

Evaluation : N=1000, type=float
COPY_ij	8.98375
COPY_ij	1.29433
COPY_ij	1.20139
COPY_ij	1.22206
COPY_ij	1.20788
COPY_ij	1.18246
COPY_ij	1.208
COPY_ij	1.19528
COPY_ij	1.19766
COPY_ij	1.19308

COPY_ji	26.3669
COPY_ji	25.1674
COPY_ji	24.7498
COPY_ji	23.1142
COPY_ji	22.5699
COPY_ji	21.8683
COPY_ji	20.8813
COPY_ji	20.4292
COPY_ji	19.9052
COPY_ji	19.2485

GCC utilise les mêmes méthodes que pour la mise à zéro, sauf en O3, où il utilise directement les instructions SIMD.

#Produit scalaire de deux vecteurs
##En fonction de la taille
###N=1000

Evaluation : N=1000, type=int
PS 	8.4113
PS 	3.80966
PS 	3.79817
PS 	3.7365
PS 	3.78668
PS 	3.77031
PS 	3.82703
PS 	3.83225
PS 	3.83509
PS 	3.92908

###N=10000

Evaluation : N=10000, type=int
PS 	5.49973
PS 	1.89659
PS 	1.9443
PS 	1.92666
PS 	1.87927
PS 	1.89815
PS 	1.88751
PS 	1.89403
PS 	1.88937
PS 	1.88091

###N=10

Evaluation : N=10, type=int
PS 	6.46
PS 	5.68
PS 	5.62
PS 	5.7
PS 	5.44
PS 	5.7
PS 	5.68
PS 	5.78
PS 	5.68
PS 	5.68

##En fonction du type
### Avec des int

Evaluation : N=1000, type=int
PS 	8.4113
PS 	3.80966
PS 	3.79817
PS 	3.7365
PS 	3.78668
PS 	3.77031
PS 	3.82703
PS 	3.83225
PS 	3.83509
PS 	3.92908

### Avec des float

Evaluation : N=1000, type=float
PS 	5.5143
PS 	3.11574
PS 	3.09099
PS 	3.0964
PS 	3.09585
PS 	3.09606
PS 	3.09552
PS 	3.08583
PS 	3.00483
PS 	3.00435

### Avec des double

Evaluation : N=1000, type=double
PS 	16.5086
PS 	6.39941
PS 	6.44422
PS 	6.24567
PS 	6.01713
PS 	6.11795
PS 	6.01834
PS 	6.12412
PS 	6.00815
PS 	5.68445

##En fonction de l'optimisation
### Pas d'optimisation (O0)

Evaluation : N=1000, type=int
PS 	21.8956
PS 	16.8019
PS 	16.1104
PS 	15.7176
PS 	15.1864
PS 	15.0313
PS 	14.235
PS 	14.2329
PS 	13.8045
PS 	13.529


``` assembly
.L47:
	movl	-4(%rbp), %eax
	cltq
	movl	BF(,%rax,4), %edx
	movl	-4(%rbp), %eax
	cltq
	movl	CF(,%rax,4), %eax
	imull	%eax, %edx
	movl	SF(%rip), %eax
	addl	%edx, %eax
	movl	%eax, SF(%rip)
	addl	$1, -4(%rbp)
.L46:
	cmpl	$999999, -4(%rbp)
	jle	.L47
```
### Optimisation O2

Evaluation : N=1000, type=int
PS 	8.4113
PS 	3.80966
PS 	3.79817
PS 	3.7365
PS 	3.78668
PS 	3.77031
PS 	3.82703
PS 	3.83225
PS 	3.83509
PS 	3.92908

``` assembly
.L69:
	movl	BF(%rax), %edx
	addq	$4, %rax
	imull	CF-4(%rax), %edx
	addl	%edx, %ecx
	cmpq	$4000000, %rax
	jne	.L69
```


### Optimisation O3
Evaluation : N=1000, type=int
PS 	5.79336
PS 	1.9622
PS 	1.95994
PS 	2.15881
PS 	2.03234
PS 	1.95082
PS 	1.95806
PS 	1.95039
PS 	1.95851
PS 	2.01547

``` assembly
.L67:
	movdqa	BF(%rax), %xmm0
	addq	$16, %rax
	movdqa	BF-16(%rax), %xmm1
	pmuludq	CF-16(%rax), %xmm0
	pshufd	$8, %xmm0, %xmm0
	movdqa	CF-16(%rax), %xmm2
	psrlq	$32, %xmm1
	cmpq	$4000000, %rax
	psrlq	$32, %xmm2
	pmuludq	%xmm2, %xmm1
	pshufd	$8, %xmm1, %xmm1
	punpckldq	%xmm1, %xmm0
	paddd	%xmm0, %xmm3
	jne	.L67
```

#Produit de matrices
##En fonction de la taille
###N=100

Evaluation : N=500, type=int
MM_ijk	3.91699
MM_ijk	2.75457
MM_ijk	2.67654
MM_ijk	2.69027
MM_ijk	2.68169
MM_ijk	2.69776
MM_ijk	2.68965
MM_ijk	2.67827
MM_ijk	2.67964
MM_ijk	2.68814

MM_ikj	1.92292
MM_ikj	1.94383
MM_ikj	1.94396
MM_ikj	1.93679
MM_ikj	1.91903
MM_ikj	1.91976
MM_ikj	1.92005
MM_ikj	1.92104
MM_ikj	1.96985
MM_ikj	1.92094

MM_B_ijk	1.98776
MM_B_ijk	1.98908
MM_B_ijk	1.98716
MM_B_ijk	1.99733
MM_B_ijk	1.98805
MM_B_ijk	2.0392
MM_B_ijk	2.01329
MM_B_ijk	1.98841
MM_B_ijk	1.99455
MM_B_ijk	1.98794

MM_TRANS	2.13281
MM_TRANS	2.14458
MM_TRANS	2.14628
MM_TRANS	2.13116
MM_TRANS	2.14283
MM_TRANS	2.1269
MM_TRANS	2.13092
MM_TRANS	2.12735
MM_TRANS	2.18782
MM_TRANS	2.13236

###N=500

Evaluation : N=500, type=int
MM_ijk	3.91699
MM_ijk	2.75457
MM_ijk	2.67654
MM_ijk	2.69027
MM_ijk	2.68169
MM_ijk	2.69776
MM_ijk	2.68965
MM_ijk	2.67827
MM_ijk	2.67964
MM_ijk	2.68814

MM_ikj	1.92292
MM_ikj	1.94383
MM_ikj	1.94396
MM_ikj	1.93679
MM_ikj	1.91903
MM_ikj	1.91976
MM_ikj	1.92005
MM_ikj	1.92104
MM_ikj	1.96985
MM_ikj	1.92094

MM_B_ijk	1.98776
MM_B_ijk	1.98908
MM_B_ijk	1.98716
MM_B_ijk	1.99733
MM_B_ijk	1.98805
MM_B_ijk	2.0392
MM_B_ijk	2.01329
MM_B_ijk	1.98841
MM_B_ijk	1.99455
MM_B_ijk	1.98794

MM_TRANS	2.13281
MM_TRANS	2.14458
MM_TRANS	2.14628
MM_TRANS	2.13116
MM_TRANS	2.14283
MM_TRANS	2.1269
MM_TRANS	2.13092
MM_TRANS	2.12735
MM_TRANS	2.18782
MM_TRANS	2.13236

###N=1000

Evaluation : N=1000, type=int
MM_ijk	19.7379
MM_ijk	19.7521
MM_ijk	19.7766
MM_ijk	19.7334
MM_ijk	19.7303
MM_ijk	19.7288
MM_ijk	19.7036
MM_ijk	19.6897
MM_ijk	19.7055
MM_ijk	19.7094

MM_ikj	1.91486
MM_ikj	1.9123
MM_ikj	1.9116
MM_ikj	1.91451
MM_ikj	1.91488
MM_ikj	1.90741
MM_ikj	1.91303
MM_ikj	1.91331
MM_ikj	1.92076
MM_ikj	1.92674

MM_B_ijk	2.07801
MM_B_ijk	2.06917
MM_B_ijk	2.06215
MM_B_ijk	2.06831
MM_B_ijk	2.07983
MM_B_ijk	2.06963
MM_B_ijk	2.0676
MM_B_ijk	2.07479
MM_B_ijk	2.07342
MM_B_ijk	2.07537

MM_TRANS	2.23725
MM_TRANS	2.23731
MM_TRANS	2.23936
MM_TRANS	2.23508
MM_TRANS	2.23287
MM_TRANS	2.23878
MM_TRANS	2.241
MM_TRANS	2.23403
MM_TRANS	2.23687
MM_TRANS	2.23378

##En fonction du type
### Avec des int

Evaluation : N=500, type=int
MM_ijk	3.91699
MM_ijk	2.75457
MM_ijk	2.67654
MM_ijk	2.69027
MM_ijk	2.68169
MM_ijk	2.69776
MM_ijk	2.68965
MM_ijk	2.67827
MM_ijk	2.67964
MM_ijk	2.68814

MM_ikj	1.92292
MM_ikj	1.94383
MM_ikj	1.94396
MM_ikj	1.93679
MM_ikj	1.91903
MM_ikj	1.91976
MM_ikj	1.92005
MM_ikj	1.92104
MM_ikj	1.96985
MM_ikj	1.92094

MM_B_ijk	1.98776
MM_B_ijk	1.98908
MM_B_ijk	1.98716
MM_B_ijk	1.99733
MM_B_ijk	1.98805
MM_B_ijk	2.0392
MM_B_ijk	2.01329
MM_B_ijk	1.98841
MM_B_ijk	1.99455
MM_B_ijk	1.98794

MM_TRANS	2.13281
MM_TRANS	2.14458
MM_TRANS	2.14628
MM_TRANS	2.13116
MM_TRANS	2.14283
MM_TRANS	2.1269
MM_TRANS	2.13092
MM_TRANS	2.12735
MM_TRANS	2.18782
MM_TRANS	2.13236

### Avec des float

Evaluation : N=500, type=float
MM_ijk	5.20435
MM_ijk	3.30093
MM_ijk	2.89574
MM_ijk	2.88814
MM_ijk	2.90193
MM_ijk	2.89037
MM_ijk	2.89153
MM_ijk	2.88571
MM_ijk	2.89844
MM_ijk	2.92001

MM_ikj	1.89504
MM_ikj	1.89708
MM_ikj	1.89794
MM_ikj	1.89506
MM_ikj	1.89889
MM_ikj	1.89549
MM_ikj	1.92836
MM_ikj	1.89584
MM_ikj	1.89548
MM_ikj	1.89509

MM_B_ijk	2.70503
MM_B_ijk	2.69963
MM_B_ijk	2.72236
MM_B_ijk	2.71164
MM_B_ijk	2.69973
MM_B_ijk	2.69998
MM_B_ijk	2.70205
MM_B_ijk	2.75266
MM_B_ijk	2.7628
MM_B_ijk	2.71108

MM_TRANS	2.7087
MM_TRANS	2.68908
MM_TRANS	2.73467
MM_TRANS	2.74971
MM_TRANS	2.6885
MM_TRANS	2.70237
MM_TRANS	2.69027
MM_TRANS	2.74608
MM_TRANS	2.68918
MM_TRANS	2.69227

### Avec des double

Evaluation : N=500, type=double
MM_ijk	4.56131
MM_ijk	3.07024
MM_ijk	4.37963
MM_ijk	3.46045
MM_ijk	3.04301
MM_ijk	3.00445
MM_ijk	3.00178
MM_ijk	3.06071
MM_ijk	3.01561
MM_ijk	3.0051

MM_ikj	1.90246
MM_ikj	1.91262
MM_ikj	1.91885
MM_ikj	2.00869
MM_ikj	2.00785
MM_ikj	1.93776
MM_ikj	1.90573
MM_ikj	1.90306
MM_ikj	1.90463
MM_ikj	1.91213

MM_B_ijk	2.85298
MM_B_ijk	2.85969
MM_B_ijk	2.79004
MM_B_ijk	2.78982
MM_B_ijk	2.7957
MM_B_ijk	2.8439
MM_B_ijk	2.81018
MM_B_ijk	2.76802
MM_B_ijk	2.77923
MM_B_ijk	2.82563

MM_TRANS	2.75163
MM_TRANS	2.72699
MM_TRANS	2.72604
MM_TRANS	2.72636
MM_TRANS	2.77953
MM_TRANS	2.75188
MM_TRANS	2.73702
MM_TRANS	2.72392
MM_TRANS	2.72548
MM_TRANS	2.76091

#Interprétation pour la multiplication de matrices

``` c
void mm_trans() {
  int i, j, m, sum, k;
  
  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (i=0;i<N;i++)
        for (j=0;j<N;j++)
	  XT[j][i]=XF[i][j];

      for (i=0;i<N;i++)
	for (j=0;j<N;j++)
	  {
	    SF=0.0;
	    for (k=0; k<N; k++)
	      SF+=AF[i][k]*XT[j][k];
	    YF[i][j]=SF;
	  }

      benchtime=dtime() - begin;
      printf ("MM_TRANS\t%g\n", (double) benchtime*N3inv);
    }
  printf ("\n");
}
```
