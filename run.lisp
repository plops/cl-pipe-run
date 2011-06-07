
(defparameter *chan*
  (sb-ext:run-program "./cmd" '()
		      :output :stream
		      :input :stream
		      :wait nil))

(defun cmd (cmd)
 (let ((s (sb-ext:process-input *chan*)))
   (write-line cmd s)
   (finish-output s)))
#+nil
(cmd "fun 3")

(defun send-binary ()
 (with-open-file (e "binary" :direction :output
		    :if-exists :append
		    :if-does-not-exist :error
		    :element-type '(unsigned-byte 8))
   (let* ((s (sb-ext:process-input *chan*))
	  (n 4)
	  (buf (make-array n :element-type '(unsigned-byte 8))))
     (setf (aref buf 0) 12)
     (write-line (format nil "get ~a" n) s)
     (finish-output s)
     (write-sequence buf e)
     (finish-output e))))
#+nil
(send-binary)


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
