.PHONY: all
all: fastq_reader
targets=fastq_reader G
CXX := g++

INC =-I Boost1.50

DEBUG_FLAGS = -O0 -g3 

NOASSERT_FLAGS = -DNDEBUG

CXXFLAGS = -O3 $(INC)
DEBUG_CXXFLAGS = -ggdb $(INC)
FIND :=$(shell which find)
# default link with mac libraries
LINKS = $(shell $(FIND) libs/mac -type f)

ifneq (,$(findstring Linux,$(shell uname)))
LINKS = $(shell $(FIND) libs/linux -type f)
endif
SRC_DIR = src

GENERAL_LIST = \
			   $(shell $(FIND) libs) \
			   $(shell $(FIND) scripts) \
			   $(wildcard *.md) \
			   $(wildcard *.rst) \
			   Makefile LICENSE VERSION AUTHORS 
SRC_PKG_LIST = $(shell $(FIND) src) \
               $(shell $(FIND)  Boost1.50) \
               $(GENERAL_LIST)
ALL_FILES_BOOST = $(shell $(FIND) Boost1.50 -name "*.hpp")
ALL_FILES =  $(ALL_FILES_BOOST)
VERSION = $(shell cat VERSION)
fastq_reader: $(SRC_DIR)/fastq_reader.cc $(SRC_DIR)/readRecord.h $(SRC_DIR)/seq.h $(ALL_FILES) 
	@echo compiling: $@
	$(CXX) $(CXXFLAGS) -o $@ $< $(LINKS) 
	@echo compiling: $@ done
	rm -rf bin
	mkdir bin
	mv $@ bin
debug: $(SRC_DIR)/fastq_reader.cc $(SRC_DIR)/readRecord.h $(SRC_DIR)/seq.h $(ALL_FILES) 
	$(CXX) $(DEBUG_CXXFLAGS)  -o fastq_reader $(SRC_DIR)/fastq_reader.cc $(LINKS) 

fastq_reader.tgz: $(SRC_PKG_LIST)
	rm -rf fastq_reader.tgz
	rm -rf fastq_reader-*
	rm -rf tmp
	mkdir tmp
	mkdir tmp/fastq_reader-$(VERSION)
	tar -czvf tmp.tgz $(SRC_PKG_LIST)
	mv tmp.tgz tmp/fastq_reader-$(VERSION)
	cd tmp/fastq_reader-$(VERSION) ; tar -xzvf tmp.tgz ; rm -f tmp.tgz
	cd tmp ; tar -czvf $@ fastq_reader-$(VERSION)
	cp tmp/$@ .
	rm -rf tmp
.PHONY: clean
clean:
	rm -rf fastq_reader-*
	rm -rf fastq_reader.tgz
	rm -rf *.swp
	rm -rf $(targets)
	rm -rf bin
	rm -f Alignment_log*
	rm -f with_5_adaptor no_5_adaptor
	rm -rf fastq_reader.dSYM
	rm -rf Quality_trimmer.dSYM
	rm -rf Guess_fastq_format.dSYM
