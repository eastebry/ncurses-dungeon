TARGET = level1
LIBS = -lm -lcurses
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean container push

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

PROJECT_NAME := ctf-testing
REGISTRY := gcr.io

# The full tag of the conatiner
CONTAINER_TAG := ${REGISTRY}/${PROJECT_NAME}/cast

# Build the docker conatiner with the appropriate tag, allowing us to push it to our registry
# The --no-cache argument builds the container from scratch, which is slow.
container:
	docker build --tag ${CONTAINER_TAG} .

# Push the container to the google container registry.
# This is the first step in deploying the challenge
push:
	gcloud docker -- push ${CONTAINER_TAG}

clean:
	-rm -f *.o
	-rm -f $(TARGET)
