MODULES =  main dispatcher exports clang_adaptor indexer includes

OBJDIR = .objects
SRCS = ${MODULES:=.c}
OBJS = $(addprefix $(OBJDIR)/,${SRCS:.c=.o})
DEPS = $(addprefix $(OBJDIR)/,${SRCS:.c=.d})

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(OUTPUT_OPTION) -MMD -MP -c $<

$(OBJDIR):
	-mkdir $(OBJDIR)

-include $(OBJDIR)/*.d
