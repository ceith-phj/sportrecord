NAME := ui_SportRecord

$(NAME)_VERSION    := 0.1.0
$(NAME)_SUMMARY    := ui_SportRecord
MODULE             := ui_SportRecord

$(NAME)_COMPONENTS := packages.apps.sports

$(NAME)_INCLUDES :=  ./include \
					 ./include/container \
					 ./include/record

$(NAME)_SOURCES     += src/*.c \
					   src/*/*.c\
					   src/*/*/*.c\
					   src/*/*/*/*.c


GLOBAL_INCLUDES 	+= ./include