(setq atcoder-library-list
      '("NONE" "cmpNaive" "UnionFind" "UnionFindRel" "bit" "coordCompr" "debug"
	"fix" "FuncGraph" "bridge" "offDynConn" "UndoUnionFind"
	"integer" "matrix" "misc" "mod" "segTree" "fenwick" "sieve" "strSearch" "algOp"
	"util" "scc" "geometry" "binsearch" "rerror" "rational"
	"suffixArray" "board" "minCostFlow" "perm" "guessPoly" "ftwo"
	"trie" "tree" "rollingHash" "fft" "doubling" "ipoint"
	"interactive" "intervalMap" "polynomial" "convolution" "adjMatrix"
        "interpolation" "fastTransform" "sortedIndex" "forall" 
        "mapget" "mypair" "digit" "topord" "input" "cycledetect" "rbst" "mo" "unordered_map" "rle" "random"
	"power" "vicinity" "sqrtDecomp" "bellmanford" "wavelet" "SternBrocot" "smallVec" "cartesianTree"
        "persSegTree" "disjSparseTbl" "doublyLinkedList" "swag" "LiChao"
        "streamOut" "streamIn" "updMaxMin" "gcd" "intDiv" "perfMeas" "f:floorSum"
	))

(setq atcoder-topdir "/home/y-tanabe/proj/compprog")
(setq atcoder-insertion-mark "@@ !! LIM")

(defun atcoder-fn-insert-library (arg)
  (let ((s1 nil) (e1 nil) (rc nil))
    (save-excursion
      (beginning-of-buffer)
      (search-forward atcoder-insertion-mark)
      (setq s1 (line-beginning-position))
      (end-of-buffer)
      (search-backward atcoder-insertion-mark)
      (setq e1 (line-beginning-position 2))
      (setq rc (shell-command-on-region s1 e1
					(concat "libins '" arg "'") 1 t)))
    (if (= rc 0)
	(progn
	  (let ((fpath (concat atcoder-topdir "/clib/" arg ".cc"))
		(sbuf (current-buffer)))
	    (if (file-exists-p fpath)
		(progn
		  (find-file-read-only-other-window fpath)
		  (switch-to-buffer-other-window sbuf))))
	  (message "Successfully inserted."))
      (progn (message "*** libins FAILED.  Consider UNDO.")
	     (goto-char s1)))))
  
(defun atcoder-insert-library (arg)
  "Insert a library file content.
Lines containing the insertion mark, \"@@ !! LIM\", are searched
from the beginning and the end, the surronded text are passed to
command libins, and the result replaces the text.

A space-separated and parentheses-surrounded list following the
insertion mark and in the same line is the list of already
inserted library contents.  Items in the form of \"f:xxx\" are
functions, others are files.

If you just want to update the contents withoug adding new items,
call this function with argument \"NONE\".

For the library file format, consult the help message of command libins."
  (interactive (list (completing-read "Library name: " atcoder-library-list)))
  (atcoder-fn-insert-library arg))

(defun atcoder-input-expand ()
  (interactive)
  (let ((orig-pos (point))
        (new-pos nil))
    (save-excursion
      (let* ((start-old (line-beginning-position))
             (spec (buffer-substring start-old (line-end-position)))
             (buf (get-buffer-create "*atcoder-expand*")))
        (with-current-buffer buf (erase-buffer))
        (if (= (call-process "inputExpand" nil buf nil spec) 0)
            (let ((expanded (with-current-buffer buf (buffer-substring (point-min) (point-max))))
                  (idx (string-match "\\[[A-Za-z0-9]\\{8\\}\\]" spec)))
              (if idx
                  (let ((case-fold-search nil))
                    (end-of-line)
                    (search-forward (concat "@End " (substring spec idx (+ idx 10))) nil t)))
              (let ((end-old (line-beginning-position 2)))
                (goto-char start-old)
                (delete-region start-old end-old)
                (insert expanded)
                (c-indent-line)
                (setq new-pos (point))
                (message "Successfully expanded.")))
          (let ((cbuf (current-buffer)))
            (switch-to-buffer-other-window buf)
            (switch-to-buffer-other-window cbuf)
            (message "expansion failed.  See *atcoder-expand*")))))
    (goto-char (or new-pos orig-pos))))

(global-set-key "\e\ei" 'atcoder-insert-library)
(global-set-key "\e\ep" 'atcoder-input-expand)
