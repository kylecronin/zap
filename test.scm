

(define (iden fn) (lambda args (apply fn args)))


(define fact (iden (lambda (x) (if (= x 0) 1 (* x (fact (- x 1)))))))