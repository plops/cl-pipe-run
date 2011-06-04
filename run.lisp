
(defparameter *chan*
  (sb-ext:run-program "./cmd" '()
		      :output :stream
		      :input :stream
		      :wait nil))
(let ((s (sb-ext:process-input *chan*)))
  (write-line "end" s)
  (finish-output s))

(write-line "blub")

(format (sb-ext:process-input *chan*) "3.2 12 31~%")

(sb-ext:process-close *chan*)

(sb-thread:make-thread 
 #'(lambda ()
     (let ((s (sb-ext:process-output *chan*)))
       (loop for line = (read-line s nil nil)
	  while line do
	    (format t "tttt ~a~%" line))))
 :name "stdin-reader")

;;http://compgroups.net/comp.lang.lisp/Redirecting-program-output-with-SBCL-s-RUN-PROGRAM
(let ((ls-process (sb-ext:run-program "/bin/ls" '() 
				      :wait nil 
				      :output :stream))) 
  (unwind-protect 
       (with-open-stream (s (sb-ext:process-output ls-process)) 
	 (let ((grep-process (sb-ext:run-program "/bin/grep" '("lisp") 
						 :input s 
						 :output :stream))) 
	   (when grep-process 
	     (unwind-protect 
		  (with-open-stream (o (sb-ext:process-output grep-process)) 
		    (loop 
		       for line = (read-line o nil nil) 
		       while line 
		       collect line)) 
	       (sb-ext:process-close grep-process))))) 
    (when ls-process (sb-ext:process-close ls-process)))) 
