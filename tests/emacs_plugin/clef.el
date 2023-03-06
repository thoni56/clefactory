(use-package lsp-mode
  :ensure t
  :defer t
  :hook (lsp-mode . (lambda ()
                      (let ((lsp-keymap-prefix "C-c l"))
                        (lsp-enable-which-key-integration))))
  :config
  (define-key lsp-mode-map (kbd "C-c l") lsp-command-map))

(lsp-register-client
 (make-lsp-client
  :new-connection (lsp-stdio-connection '("clef" "--lsp" "--log=log" "--trace"))
  :major-modes '(c-mode)
  :priority 0
  :activation-fn (lsp-activate-on "c")
  :server-id 'lsp-clef))

(setq lsp-client-packages (cons 'lsp-clef lsp-client-packages))
