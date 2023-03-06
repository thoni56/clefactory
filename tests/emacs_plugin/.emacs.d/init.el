(require 'package)
;;; Add some Melpa archive
(add-to-list 'package-archives '("melpa" . "http://melpa.org/packages/") t)
(package-initialize)

(use-package lsp-mode)

(load-file "clef.el")
