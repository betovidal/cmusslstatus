MAKE    :=make
LDFLAGS :=-std=c99 -pedantic -Wall -Wextra -Os
CC      :=gcc
LDIRS   :=
IDIRS   :=-I.
OUTNAME :=query-cmus
MAIN    :=query-cmus
RM      :=rm -f
EXECUTE :=./$(OUTNAME)
UNION   :=&&
ifeq ($(OS),Windows_NT)
	LDIRS      :=
	IDIRS      :=
	OUTNAME    :=
	LDFLAGS    :=
	RM         :=
	MAKE       :=
	EXECUTE    :=
	UNION      :=
endif
SRCFILES := $(wildcard *.c)
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))
DEPFILES := $(patsubst %.c,%.d,$(SRCFILES))
WARNINGS := -Wall
CFLAGS   := -std=c99 -pedantic $(WARNINGS) $(LDFLAGS)
# Let's see if I remember correclty... first recipe will be:
$(OUTNAME): $(OBJFILES)
	$(CC) $(LDIRS) $(MAIN).o $(subst $(MAIN).o,,$(OBJFILES)) -o $@ ${CFLAGS}
$(OBJFILES): $(SRCFILES)
	$(CC) $(IDIRS) -c $?
clean:
	$(RM) $(OBJFILES)
	# $(RM) $(DEPFILES)
	$(RM) $(OUTNAME)
run:
	$(MAKE) $(UNION) $(EXECUTE)
