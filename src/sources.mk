MODULES = main dispatcher exports parser includes	\
	references filemanager fileio units repl lsp server_handler process \
	options log clang_adapter json_adapter

OBJDIR = .objects
SRCS = ${MODULES:=.c}
OBJS = $(addprefix $(OBJDIR)/,${SRCS:.c=.o})
DEPS = $(addprefix $(OBJDIR)/,${SRCS:.c=.d})

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(OUTPUT_OPTION) -MMD -MP -c $<

$(OBJDIR):
	-mkdir $(OBJDIR)

-include $(OBJDIR)/*.d
