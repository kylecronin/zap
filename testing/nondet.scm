(define *paths* '())
 
(define (choose choices)
  (call/cc
   (lambda (cc)
     (set! *paths*
           (append *paths*
                   (map (lambda (choice)
                          (lambda () (cc choice)))
                        choices)))
     (fail))))
 
(define (fail)
  (if (null? *paths*) 'fail
      (let ((p1 (car *paths*)))
        (set! *paths* (cdr *paths*))
        (p1))))
 
(define (resolve exp data)
  (or
   (eq? 0 (car data))
   (and (car exp) (car data))
   (and (not (car exp)) (not (car data))) 
   (if (null? (cdr exp)) #f
       (resolve (cdr exp) (cdr data)))))
 
(define (eval-exp exp data)
  (if (null? data) #t
      (and (resolve exp (car data)) (eval-exp exp (cdr data)))))
 
(define (make-choices num options)
  (if (= num 0) '()
      (cons (choose options) (make-choices (- num 1) options))))
 
(define (satisfy cnf)
  (let* ((options '(#t #f))
         (choice (make-choices (length (car cnf)) options)))
    (if (eval-exp choice cnf) choice
        (fail))))

(satisfy '((#f)))

