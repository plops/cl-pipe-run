
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

