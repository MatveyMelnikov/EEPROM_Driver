TESTS = Y
FOR_TARGET = N


ifeq ($(TESTS), Y)
include MakefileTest.mk
else # For target only
include MakefileProduction.mk
endif
