;; Clef LSP definitions
(require 'lsp)

(add-to-list 'lsp-client-packages 'lsp-clef)

(lsp-register-client
 (make-lsp-client
  :new-connection (lsp-stdio-connection '("clef" "--lsp" "--log=log" "--trace"))
  :major-modes '(c-mode)
  :priority 0
  :activation-fn (lsp-activate-on "c")
  :server-id 'lsp-clef))
