# Guess the Number Game

## Structure

```
homework_03
   play                 Parser, main entrypoint
   guess_the_number     Game workflow logic
   records              Table of records I/O logic
   agent                Binary search agent for auto play
   tests                Manual tests

```

## Usage

Run game in an interactive mode
```
./guess_the_number -level 3
```

Run game in an auto-played mode (via binary search)
```
./guess_the_number -auto
```

Show table of records
```
./guess_the_number -table
```
<p align="center">
<img width="634" alt="Screen Shot 2022-05-04 at 23 10 50" src="https://user-images.githubusercontent.com/23639048/166818693-6073945f-cfb4-4587-b706-2facc64aff5d.png">
</p>

Run manual tests
```
./guess_the_number -tests
```
