
(defparameter *chan*
  (sb-ext:run-program "./cmd" '()
		      :output :stream
		      :input :stream
		      :error :stream
		      :wait nil))

(let ((s (sb-ext:process-input *chan*)))
  (write-line "dump 4" s)
  (finish-output s))

(progn
 (defvar *dump* nil)
 (sb-thread:make-thread 
  #'(lambda ()
      (with-open-stream (s (sb-ext:process-error *chan*))
	(loop
	   (let ((a (make-array 4 :element-type '(unsigned-byte 8))))
	     (read-sequence a s)
	     (format t "dump ~a.~%" a)
	     (setf *dump* a)))))
  :name "dump-reader"))

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
