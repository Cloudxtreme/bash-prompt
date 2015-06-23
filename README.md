# bash-prompt
Generates my bash prompt.

This prompt shows the host name, and the full path of the working directory if
this path is less than 25 chars. Otherwise the full path is replaced by the name
of the working directory.

To compile the source:
```
gcc bash_prompt.c -o bash_prompt
```

I configured bash to run this program before displaying a prompt. The output of
this program is assigned to the `PS1` variable.

`.bash_profile` includes:
```
function setup_prompt {
  export PS1=$(~/bin/bash_prompt)
}

export PROMPT_COMMAND=setup_prompt
```
