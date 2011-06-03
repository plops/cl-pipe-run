
(defparameter *chan*
  (sb-ext:run-program "./cmd" '()
		      :output :stream
		      :input :stream
		      :wait nil))
(dotimes (i 1024)
 (format (sb-ext:process-input *chan*) "fun ~d 31~%" i))
(format (sb-ext:process-input *chan*) "3.2 12 31~%")

(sb-ext:process-close *chan*)

(format t
 (read-line (sb-ext:process-output *chan*)))

