;;;
;;; $Id$
;;;
;;; to load analyzer environment inside Le-Lisp session
;;;

;; load top-module
(newr #:system:path (cond (#:system:unixp
			   (catenate #:system:directory "ll2lm/"))
			  (#:system:dosp
			   (catenate #:system:directory "ll2lm\"))
                          (#:system:vaxvmsp
			   "lelisp$disk:[LL2LM]")
                          (t "")))
crunch

;; no defextern at analyze-time
(unless #:system:dosp
        (unless (getdef '#:crunch:getglobal)
                (synonymq #:crunch:getglobal getglobal))
        (defun getglobal (x) 1)
        (defun defextern-cache l))

