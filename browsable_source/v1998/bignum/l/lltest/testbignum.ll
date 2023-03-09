;;.EnTete "Le-Lisp (c) version 15.2" " " "Test du package BigNum"
;;.EnPied "testratio.ll" "%" " "
;;
;;.SuperTitre "Test du package BigNum"
;;

;;.Section "De'clarations"
;;; On charge le package de test si besoin.
(unless (featurep 'testcomm)
        (libload testcomm) )

;;; On charge le package BigNum si besoin.
(unless (featurep 'q)
        (loadmodule 'bnq))

;;; On passe en mode test.
(testfn ())

;;.Section "Lecture"
    (test-serie "Test de lecture." ())

     12                               12
     12345678910                      12345678910
     00000123                         123
     -123                             -123
     -32768                           -32768
     2/3                              2/3
     4/6                              2/3
     -10/2                            -5
     (setq a 52/4096)                 13/1024
    (setq #:system:print-for-read t)  t
    #{12121212112121212112A-90349
                         39049340} #{121212121121212121129034939049340}
    (setq lec 121202943034923942303942034) #{121202943034923942303942034}
    (setq exlec (explode lec))  (35 123 49 50 49 50 48 50 57 52 51 48 51 
				 52 57 50 51 57 52 50 51 48 51 57 52 50 
				 48 51 52 125)
    (implode exlec)  #{121202943034923942303942034}
    (setq #:system:print-for-read ())    ()

;;.Section "Pre'dicats"
     (test-serie "Test des pre'dicats." ())

     (integerp 2)                      2
     (integerp -2)                    -2
     (integerp 123456234567345678)     123456234567345678
     (integerp -123456234567345678)   -123456234567345678
     (integerp 4/2)                    2
     (integerp -4/2)                  -2
     (integerp 4/3)                    ()
     (integerp -4/3)                   ()
     (integerp 2.0)                    ()
     (integerp -2.0)                   ()
     (integerp 2.3)                    ()
     (integerp -2.3)                   ()

     (numberp 2)                       2
     (numberp -2)                     -2
     (numberp 123456234567345678)      123456234567345678
     (numberp -123456234567345678)    -123456234567345678
     (numberp 4/2)                     2
     (numberp -4/2)                   -2
     (numberp 4/3)                     4/3
     (numberp -4/3)                   -4/3
     (numberp 4/6)                     2/3
     (numberp -4/6)                   -2/3
     (numberp 2.0)                     2.0
     (numberp -2.0)                   -2.0
     (numberp 2.3)                     2.3
     (numberp -2.3)                   -2.3

     (rationalp 2)                     2
     (rationalp -2)                   -2
     (rationalp 123456234567345678)    123456234567345678
     (rationalp -123456234567345678)  -123456234567345678
     (rationalp 4/2)                   2
     (rationalp -4/2)                 -2
     (rationalp 4/3)                   4/3
     (rationalp -4/3)                 -4/3
     (rationalp 4/6)                   2/3
     (rationalp -4/6)                 -2/3
     (rationalp 2.0)                   ()
     (rationalp -2.0)                  ()
     (rationalp 2.3)                   ()
     (rationalp -2.3)                  ()

;;.Section "Comparaisons"
     (test-serie "Test des fonctions de comparaisons." ())

     (<?> 2 4/2)                       0
     (<?> 3/2 2/3)                     1
     (<?> 2/3 3/4)                    -1
     (<?> 123456. 123456)              0
     (<?> (- (** 2 15) 1) 32767)       0
     (<?>  (- 123458 123457) 1)        0
     (> 36764 0)      		       36764
     (/= 3 3)                          ()

;;.Section "Addition - Soustraction"
     (test-serie "Test des fonctions d'additions et de soustractions." ())

     (1+ 99999)                       100000
     (1+ -99999)                     -99998
     (1+ 5/7)                         12/7
     (1+ -5/7)                        2/7
     (1+ 4/6)                         5/3
     (1+ -4/6)                        1/3

     (1- 99999)                       99998
     (1- -99999)                     -100000
     (1- 7/2)                         5/2
     (1- 1/2)                         -1/2
     (1- 4/6)                        -1/3
     (1- -4/6)                       -5/3

     (+ -3 100000)                    99997
     (+ 100000 -3)                    99997
     (+ -3 -100000)                  -100003
     (+ -100000 -3)                  -100003
     (+ -16384 -16384)               -32768
     (+ -16383 -16385)               -32768
     (+ -1684 12345678)               12343994
     (+ 12345 123456)                 135801
     (+ 123456 12345)                 135801
     (+ 123456123456 12345)           123456135801
     (+ 12345 123456123456)           123456135801
     (+ 12345 -123456123456)         -123456111111
     (+ 123456123456 -12345)          123456111111
     (+ 12345 -123456)               -111111
     (+ 123456 -12345)                111111
     (+ 1 2 1/2)                      7/2
     (+ 1 -1/2 1/3)                   5/6
     (+ 1/3 2/3)                      1
     (+ (- (** 2 32) 1) 1)            4294967296
     (+ 32768 2.)                     32770.

     (- 32768)                       -32768
     (- 1000000 1)                    999999
     (- 1 1000000)                   -999999
     (- 16384 12345678)              -12329294
     (- 12345 -123456)                135801
     (- 123456 -12345)                135801
     (- 12345 -123456123456)          123456135801
     (- 123456123456 -12345)          123456135801
     (- 12345 123456)                -111111
     (- 123456 12345)                 111111
     (- 12345 123456123456)          -123456111111
     (- 123456123456 12345)           123456111111
     (- -3/2)                         3/2
     (- 3/2)                          -3/2
     (- 1/2 1/3 1)                    -5/6
     (- 5/3 2/3)                      1
     (- 1000000 1)                    999999
     (- 1/100000)                    -1/100000

;;.Section "multiplication"
     (test-serie "Test des fonctions de multiplications." ())

     (* 2 -16384)                    -32768
     (* 12345679 81)                  999999999
     (* -100000000000 -100000000000)  10000000000000000000000
     (* -100000000000 100000000000)  -10000000000000000000000
     (* 100000000000 -100000000000)  -10000000000000000000000
     (* 100000000000 100000000000)    10000000000000000000000
     (* 48270948888581289062500000000 
        845870049062500000) 
          40830949904677684825316369628906250000000000000
     (* 6956883693 3258093801689886619170103176686855)
          22666179639240748063923391983020279316955515
     (* 1/2 -1/3 1/4)                -1/24
     (* 5/3 6/25)                     2/5
     (* 22/7 7/11)                    2
     (* 22/7 7/11 -1/3)              -2/3

     (** 2 128)   		      340282366920938463463374607431768211456
     (** 10 10)   		      10000000000
     (** 2/3 10)                      1024/59049
     (** 2/3 0)                       1
     (** 0 2/3)                       0
     (** 1 2/3)                       1
     (** 2 -3)                        1/8
     (** 2 -6)                        1/64
     (** 8 1/3)                       2.

;;.Section "Division"
     (test-serie "Test des fonctions de division." ())

     (/ 1 24)                         1/24
     (/ 5/3 2/3)                      5/2
     (/ 1/3)                          3
     (/ 1. 100000)                    1.e-05
     (/ 999999999 81)                 12345679

     (quotient -32768 2)             -16384
     (quotient 4881014060 4)          1220253515
     (quotient 264195 97200)          2
     (quotient 1234567890 12345678901234567890) 0
     (quotient 3000 (- 1234567891234 1234567890000)) 2
     (quotient   22685491128062564230891640495451214097
                 5281877500950955845296219748)        4294967295
     (quotient 1234567890 1234567890) 1
     (quotient 7/2 3)                 1
     (quotient 7/2 -3)               -1
     (quotient 3 2/5)                 7
     (quotient -3 2/5)               -8
     (quotient 22/7 3)                1
     (quotient -22/7 3)              -2
     (quotient 22/7 113/47)           1
     (quotient 10011100.23 1)         10011100
     (float (quotient 10011100.23 1000))    10011.

     (modulo -1234567890 1234567899)  9
     (modulo (- 12345678900000 12345678926887) 3) 2
     (modulo 38705419208160503264676104110080 607823) 101593
     (modulo 1234567890 12345678901234567890) 1234567890
     (modulo 3000 (- 1234567891234 1234567890000)) 532
     (modulo 1234567890 1234567890)   0
     (modulo 97200 69795)             27405
     (modulo 7/2 3)                   1/2
     (modulo 7/2 -3)                  1/2
     (modulo 3 2/5)                   1/5
     (modulo -3 2/5)                  1/5
     (modulo 22/7 3)                  1/7
     (modulo -22/7 3)                 20/7
     (modulo 22/7 113/47)             243/329

     (setq a 1234567)                 1234567
     (setq b 123456)                  123456
     (+ (* (quotient a b) b) (modulo a b))                        1234567
     (+ (* (quotient (- a) (- b)) (- b)) (modulo (- a) (- b)))   -1234567
     (+ (* (quotient a (- b)) (- b)) (modulo a (- b)))            1234567
     (+ (* (quotient (- a) (- b)) (- b)) (modulo (- a) (- b)))   -1234567


     (quomod -100000 12342342345)                                -1
     (+ (* 113/47 (quomod 22/7 113/47)) #:ex:mod)                 22/7
     (+ (* -113/47 (quomod 22/7 -113/47)) #:ex:mod)               22/7
     (quomod -100000 12342342345)                                -1
     (progn (quotient 1234565 101)
	    (* 100000000000000 100000000000000)
	    #:ex:mod)                                             42

     (gcd 1769 551)                                               29
     (gcd 264195 97200)                                           135
     (gcd 1769 551)                                               29
     (gcd 12432245661452 314523541234)                            2

     (setq fauxzero  (- 10000000000000 10000000000000))           0
     (modulo  fauxzero 1000000000000)                             0
     (quotient  fauxzero 1000000000000)                           0
     (progn (modulo  fauxzero 1000000000000) #:ex:mod)            0
     (modulo (- fauxzero) 10000000000000)                         0
     (quotient (- fauxzero) 10000000000000)                       0
     (gcd  fauxzero 10000000000000)                         10000000000000
     (type-of fauxzero)                                           fix

;;.Section "Division par ze'ro"
     (test-serie "Test de la division par 0." ())

     1/0           		      1/0
    -2/0          		     -1/0
     (1+ -2/0)                       -1/0
     (+ -1/0 2/3)                    -1/0
     (+ 1/0 1/0)                      1/0
     (+ 1/0 -1/0)                     0/0
     (- 1/0)                         -1/0
     (- 0/0)                          0/0
     (- 1/0 1/0)                      1/0
     (* -1/0 1/0)                    -1/0
     (* 1/0 0)                        0/0
     (** 0 0)                         0/0
     (/ -1/0)                         0

;;.Section "Conversion"
     (test-serie "Test des fonctoins de conversions." ())

     123456/7                         123456/7
     123456/7                         17636.57142857142857142857
     (truncate 123456/7)              17636
     (truncate -123456/7)            -17636
     (truncate 2/3)                   0
     (truncate -2/3)                  0
     (truncate 123456)                123456
     (truncate -123456)              -123456
;     (truncate (power 2 50))          1125899906842624
     (truncate (power 2 20))          1048576
;     (truncate (power 10 10)))        10000000000
;     (truncate (- (power 2 50)))     -1125899906842624
     (truncate (- (power 2 20)))     -1048576
;     (truncate (- (power 10 10))))   -10000000000
     (truncate 123.456789)            123
     (truncate 1234.56789)            1234
     (truncate 12345.6789)            12345
     (truncate 123456.789)            123456
     (truncate 1234567.89)            1234567
     (truncate 12345678.9)            12345678
     (truncate -123.456789)          -123
     (truncate -1234.56789)          -1234
     (truncate -12345.6789)          -12345
     (truncate -123456.789)          -123456
     (truncate -1234567.89)          -1234567
     (truncate -12345678.9)          -12345678
;     (truncate -14232453423.123423)  -14232453423
     (fdiv (float (truncate (power 2 50))) 1000000000000000.)      1.1259
     (fdiv (float (truncate (power 10 10))) 10000000000.)          1.00
     (fdiv (float (truncate (- (power 2 50)))) 1000000000000000.) -1.1259
     (fdiv (float (truncate (- (power 10 10)))) 10000000000.)     -1.

     (abs 7/2)                        7/2
     (abs -1/2)                       1/2

     (numerator -4/3)                -4
     (denominator -4/3)               3
     (numerator 7)                    7
     (denominator 7)                  1

;;.Section "Fonction trigo"
     (test-serie "Test des fonctions trigonomiques." ())

     (exp 1/3)                        1.39561242508608952862
     (exp 123456)                     1.7e+38
     (exp -123456)                   -1.7e+38
     (exp 230.25)                     9915268022112193112773804101189156238276877088319739277383448394081381854101321126361892449463681647.60477040681493387718
     (exp -19)                        .00000000560279643755
     (exp 2/3)                        1.94773404105467585662
     (log .00000000560279643755)     -18.99999999999772748124
     (log 1/3)                       -1.09861228866810969140
     (log 123456)                     11.72364009626540093516
     (atan 1/3)                       .32175055439664219339
     (atan 123456)                    1.57078822674305646460
     (atan -123456)                  -1.57078822674305646460
     (sin 1/3)                        .32719469679615224417
     (sin 123456)                    -.7402834538866575367
     (sin -123456)                    .7402834538866575367
     (asin 1/3)                       .3398369094541219
     (cos 1/3)                        .94495694631473766438
     (cos 123456)                    -.67229488165658451960
     (cos -123456)                   -.67229488165658451960
     (acos 1/3)                       1.230959417340775

;;.Section "Fonctions de haut niveau"
     (test-serie "Test des fonctions de haut niveau." ())

     (fact 42)  1405006117752879898543142606244511569936384000000000
     (modulo (fact 13) (fact 9))       0
     (modulo (fact 34) (fact 13))      0
     (modulo (fact 34) (fact 24))      0
     (modulo (fact 57) (fact 21))      0
     (modulo (fact 40) (fact 39))      0  
     (quotient (fact 59) (fact 58))    59

     (fib 250)  7896325826131730509282738943634332893686268675876375

(de zeta (n e)
    (let ((r 0))
         (for (i 1 1 n)
              (setq r (+ r (1/ (** i e)))))
         r))                       zeta
     (zeta 10 1)  		   7381/2520
     (zeta 10 2)  		   1968329/1270080
(de serie-e (n)
    (let ((e 1))
         (for (i 1 1 n) (setq e (+ e (1/ (fact i)))))
         e))                       serie-e
     (serie-e 20)  		   6613313319248080001/2432902008176640000
     (float (setq e (serie-e 18))) 2.71828182845904523536
     (+ e 0.0)                     2.71828182845904523536
     (* e 1.0)                     2.71828182845904523536
     (/ e 1.0)                     2.71828182845904523536

;;.Section "This is the end, my friend"
     (test-serie "Fin du test" ())

     ()                            ()