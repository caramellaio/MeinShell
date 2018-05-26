# Interactive Shell

A shell implementation for operating system course.

## Structure

```
bin/
src/
    argument_handling/
    command_processor/
    log/
        logger_process/
    shell/
    main.c
```


## Install

From GCC

``` bash
$ make build
```

## Clean project

``` bash
$ make clean
```

## Usage

``` bash
$ cd bin/
$ ./shell -l Logger -c -o <outputFile> -e <errorFile>
```

#### Options Details

    --help | -h, has_arg=false
    --out-file | -o, has_arg=true
    --err-file | -e, has_arg=true
    --code | -c, has_arg=false
    --max-size | -m, has_arg=true
    --logger-path | -l, has_arg=true

## Authors

>- Alberto Bombardelli
>- Anna Pedrotti
>- Stefano Sega
>- Sabrina Alexandra Rosso