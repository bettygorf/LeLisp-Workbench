=copy loader.ll,#
;==========================================================================
;
;                  Le_Lisp 68K  :  chargeur et assembleur LLM3
;
;===========================================================================
;  (c) 1982 Institut National de Recherche en Informatique pseudo END liste la liste
;   des references externes non resolues.
; - les etiquettes locales doivent etre resolues a la fin de la liste
;   des instructions.
; - pour pouvoir charger le compilateur et le chargeur, le changement
;   effectif de la FVAL et ************************************************************
;*        Les variables globales du chargeur
;************************************************************
 
 
;-----    Adresses de la zone code
 
(setq ld-Bcode (:BCODE)
      ld-Ecode (:ECODE))
 
;-----    La liste des points d'entree globaux
 
(setq ld-global-list ())
 
 
;************************************************************
;*        Fonction principale de chargement
;************************************************************
 
(de loader (ld-lobj ld-talkp)
 
  --------------------> BREAK!
COPY    $ 0001 $100000C6 FROM IOS ** BREAK CONDITION 
  CMD= WRITE OPT=$0000 LU=  2

  COPY:  ABORTED BY &SCT=8009
