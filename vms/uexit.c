/*	special exit command, to exit cleanly from VMS program.

	Unix programs usually return 0 for "good execution",
	and 1 for "some problem occured."  In VMS, we WANT the
	program to return a 1, or else we get an ugly
	"%NONAME-W-NOMSG, Message number 00000000"
	message upon image exit (yeech!).

	So, we convert 0 exit codes to 1's, leaving all others alone.
*/
uexit(exitcode)
int exitcode;
{
	if(exitcode == 0)	/* On Unix, clean, on VMS, yeech! */
		exit(1);	/* converted... */
	else
		exit(exitcode); /* returned unscathed... */
}