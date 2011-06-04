
(defparameter *chan*
  (sb-ext:run-program "./cmd" '()
		      :output :stream
		      :input :stream
		      :wait nil))

(let ((s (sb-ext:process-input *chan*)))
  (write-line "fun 3" s)
  (finish-output s))

(sb-thread:make-thread 
 #'(lambda ()
     (unwind-protect
	  (with-open-stream (s (sb-ext:process-output *chan*))
	    (loop for line = (read-line s nil nil)
	       while line do
		 (format t "tttt ~a~%" line)
		 (finish-output)))
       (sb-ext:process-close *chan*)))
 :name "stdin-reader")
