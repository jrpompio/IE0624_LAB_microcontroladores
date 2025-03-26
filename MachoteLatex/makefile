TARGET = B97026_LAB1

SOURCES = $(TARGET).tex
PDF = $(TARGET).pdf
LATEX = pdflatex -shell-escape -output-directory=build

all: run

$(PDF): $(SOURCES)
	$(LATEX) $(SOURCES)
	$(LATEX) $(SOURCES)
	mv build/$(TARGET).pdf $(TARGET).pdf


run: $(PDF)
	okular $(PDF)

clean:
	rm -rf build/*
