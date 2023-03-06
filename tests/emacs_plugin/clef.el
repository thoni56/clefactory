;; Clef LSP definitions
(require 'lsp)

(setq lsp-client-packages (cons 'lsp-clef lsp-client-packages))

(lsp-register-client
 (make-lsp-client
  :new-connection (lsp-stdio-connection '("clef" "--lsp" "--log=log" "--trace"))
  :major-modes '(c-mode)
  :priority 0
  :activation-fn (lsp-activate-on "c")
  :server-id 'lsp-clef))
