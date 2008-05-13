(define (iden fn) (lambda args (apply fn args)))

(define fact (iden (lambda (x) (if (= x 0) 1 (* x (fact (- x 1)))))))

(define (memoize fn) (letrec ((get (lambda (val) '(#f))) (set (lambda (key val) (let ((old-get get)) (set! get (lambda (query) (if (equal? query key) (cons #t val) (old-get query)))))))) (lambda args (let ((ret (get args))) (if (car ret) (cdr ret) (let ((ans (apply fn args))) (begin (set args ans) ans))))))) 

(define fib (memoize (lambda (x) (if (or (= x 1) (= x 2)) 1 (+ (fib (- x 1)) (fib (- x 2)))))))

(define (greatest a . x)
	(if (null? x) a
		(if (< a (car x))
			(apply greatest x)
			(apply greatest (cons a (cdr x))))))
							^^^^^^^^^^^^^^^^
							wasteful cons
			
