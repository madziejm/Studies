Bomb Assignment
===============

## Useful GDB commands
What I've got to know is:
 * `run`, `run <parameters>`; `exec`
 * `step`, `next` – not much useful in the task actually
 * `stepi`, `nexti` (`si`, `ni`, `si #number_of_instructions`, `ni #number_of_instructions`)
 * `frame`
 * `break <function_identifier>`, `del <breakpoint_id>`
 * `info <command>` (e.g. `info frame`, `info break`)
 * examine: `x/x $esp`, `x/d $esp`, `x/i $esi` (current instruction lookup), etc.
 * print: `print/x 1023`, `print (char*)<string_address>`, `print *(char**)<address>` (string located at address in <address>)
 * set: `set $eax=42`
 * `continue`
 
 Plasma command to print function: `plasma -i -nc a.out v function_identifier`
