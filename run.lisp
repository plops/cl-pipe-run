
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
#+nil
(cmd "end")

(defparameter *binary*
  (open "binary" :direction :output
	:if-exists :supersede
	:if-does-not-exist :error
	:element-type '(unsigned-byte 8)))

(defun send-binary ()
  (let* ((s (sb-ext:process-input *chan*))
	 (n 256)
	 (buf (make-array (list n n) :element-type '(unsigned-byte 8)))
	 (buf1 (sb-ext:array-storage-vector buf)))
    (dotimes (i n)
      (dotimes (j n)
	(let* ((x (- i (floor n 2)))
	       (y (- j (floor n 2)))
	       (r2 (+ (* x x) (* y y))))
	  (setf (aref buf j i) (if (< r2 (expt 28 2))
				   i
				   (- 255 i))))))
    (write-line (format nil "load ~a" (* n n)) s)
    (finish-output s)
    (write-sequence buf1 *binary*)
    (finish-output *binary*)))
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
