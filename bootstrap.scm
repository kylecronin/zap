(define (fact x)
	(if (= x 0) 1
		(* x (fact (- x 1)))))
		
(define (fib x)
	(if (<= x 1) x
		(+ 	(fib (- x 1))
			(fib (- x 2)))))
			
(define (<= a b)
	(or (< a b) (= a b)))
	
(define (> a b)
	(not (<= a b)))
	
(define (>= a b)
	(or (> a b) (= a b)))
	
(define (length x)
	(if (null? x) 0
		(+ 1 (length (cdr x)))))
		
(define (memoize fn comp)
	(letrec ((get (lambda (query) '(#f)))
			 (set (lambda (query value)
					(let ((old-get get))
						(set! get (lambda (q)
									(if (comp q query)
										(cons #t value)
										(old-get q))))))))
		(lambda args
			(let ((cache (get args)))
				(if (car cache)
					(cdr cache)
					(let ((call (apply fn args)))
						(set args call)
						call))))))
						
(define mfib (memoize (lambda (x)
						(if (< x 2) x
						(+ (mfib (- x 1)) (mfib (- x 2)))))
					equal?))

(define def define)

(define testc 
	(begin
		(load "test.scm")
		cube))
		
(define (reload) (load "bootstrap.scm"))

(define t 'temp)

(define (go) (+ 1 (call/cc (lambda (c) (set! t c) 1))))