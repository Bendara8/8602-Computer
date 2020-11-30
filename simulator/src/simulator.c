/* Organization
 * chip
 * - struct Chip
 *   - enum ChipType type
 *   - enum ChipState state
 *   - long delay_ctr
 *   - struct Net **in_arr
 *   - struct Net **out_arr
 *   - union
 *     - char *mem_arr
 *     - long val
 * // step functions for each chip type
 *
 * net
 * - struct Net
 *   - long curr_val
 *   - long next_val
 *
 * circuit
 * - struct Circuit
 *   - struct Chip *chip_arr
 *   - struct Net *net_arr
 * // step function for entire circuit
 *
 * display
 * // pulls state of rgbi binary output and displays color to window
 * 
 * keyboard
 * // handles key presses and interfaces with PS/2 nets
 */

/* Notes
 * Use seperate buffer for HO, no pull up resistors
 * Tie enable pin for data bus ouput to 2.5 MHz clock, that way there is no chance of two buffers outputting at one time
 * Swap ORs back to NANDs -> two extra to use in interrupt and I/O circuit
 * Use 4 NANDs for clock combination and inverting
 * Allow for multicolor text by using a color register which holds foreground and background color, pixel bit selects between the two
 * Use same pulse that latches pixel data from VRAM to latch resultant color from last pixel
 * chains of lod A # sto A P+#0 to adjust text mode foreground/background for syntax highlighting, use nop for timing/spacing
 * select y counter line to int with io reg
 */