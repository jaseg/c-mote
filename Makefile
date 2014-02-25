
HTML_OUTPUTS := $(patsubst %.rst,%.html,$(wildcard *.rst))

all: $(HTML_OUTPUTS)
	
%.html : %.rst
	rst2html $< $@
