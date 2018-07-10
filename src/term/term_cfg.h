/**
 * Compile-time configuration for the terminal interface.
 */
#ifndef TERM_CFG_H
#define TERM_CFG_H


/** Ansi escape command to clear screen. */
#define TERM_VT102_RESET "\033[2J"

/** Ansi escape command to set bright text attribute. */
#define TERM_VT102_ATTR_BRIGHT "\033[1m"

/** Ansi escape command to reset text attributes. */
#define TERM_VT102_ATTR_RESET  "\033[0m"

/** The command-line prompt to print */
#define TERM_PROMPT (TERM_VT102_ATTR_BRIGHT "> " TERM_VT102_ATTR_RESET)


#endif /* TERM_CFG_H */
