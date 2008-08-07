(define (make-queue)
  (letrec ((queue '())
           (add-to-end (lambda (item lst)
                         (if (null? lst)
                             (list item)
                             (cons (car lst)
                                   (add-to-end item (cdr lst))))))
           (enqueue (lambda (item)
                      (set! queue (add-to-end item queue))))
           (dequeue (lambda ()
                      (if (null? queue) 'queue-empty
                          (let ((ret (car queue)))
                            (set! queue (cdr queue))
                            ret)))))
    (cons enqueue dequeue)))

(define queue (make-queue))
(define enqueue (car queue))
(define dequeue (cdr queue))
  
(define (fork)
  (display "forking\n")
  (call-with-current-continuation
   (lambda (cc)
     (enqueue (lambda ()
                (cc #f)))
     (cc #t))))
  
(define (context-switch)
  (display "context switching\n")
  (call-with-current-continuation
   (lambda (cc)
     (enqueue
	(lambda ()
		(cc 'nothing)))
     ((dequeue)))))

(define (end-process)
  (display "ending process\n")
  (let ((proc (dequeue)))
    (if (eq? proc 'queue-empty)
        (display "all processes terminated\n")
        (proc))))
  
(define (mt)
  (display "entering test\n")
  (if (fork)
      (if (fork)
          (begin
            (display "process 1\n")
            (context-switch)
            (display "process 1 again\n"))
          (begin
            (display "process 2\n")
            (end-process)
            (display "you shouldn't see this (2)")))
      (if (fork)
          (begin
            (display "process 3\n")
            (display "process 3 again\n")
            (context-switch))
          (begin
            (display "process 4\n"))))
  (context-switch)
  (display "ending process\n")
  (end-process)
  (display "process ended (should only see this once)\n"))



