;;;
;;; COUNTPKGC:  print the use of the packagecell of all symbols
;;;
;;; $Source: /usr/cvs/lelisp/llub/countpkgc.ll,v $
;;; $Date: 2016/05/21 10:36:08 $
;;; $Revision: 1.2 $
;;;
;;; ------------------------------------------------------------
;;; This file is a contribution of Le-Lisp version 15.
;;;
;;;
;;; (c) 1987-1993  Le-Lisp is a trademark of INRIA.
;;; ------------------------------------------------------------



(defun countpkgc ()
  ;; print the use of the packagecells
  (let ((al (ncons (cons () 0)))
	(tot 0.))
    (mapoblist (lambda (symb)
		 (incr tot)
		 (countpkgc1 (packagecell symb) al)))
    (print "Total number of symbols: " tot)
    (mapc (lambda (slot)
	    (prinf "~S ~40T ~4D ~44T ~3D %"
		   (car slot)
		   (cdr slot)
		   (fix (/ (* (cdr slot) 100.) tot)))
	    (print))
	  (sort (lambda (x y) (if (eq (cdr x) (cdr y))
				  (pkgcmp (car x) (car y))
				  (gt (cdr x) (cdr y))))
		al))))


(defun countpkgc1 (pkgc al)
  (let ((slot (assq pkgc al)))
     (if (consp slot)
	 (rplacd slot (add1 (cdr slot)))
         (nconc1 al (cons pkgc 1))))))


(print "(countpkgc) to print the counting.")


		
