
(defparameter *chan*
  (sb-ext:run-program "./cmd" '()
		      :output :stream
		      :input :stream
		      :wait nil))
(format (sb-ext:process-input *chan*) "fun 12 31~%")
(format (sb-ext:process-input *chan*) "3.2 12 31~%")

(sb-ext:process-close *chan*)

(format t
 (read-line (sb-ext:process-output *chan*)))

