package main

import (
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"strings"
	"unicode"

	svg "github.com/ajstarks/svgo"
)

var (
	comboFile = flag.String("combos", "", "combos.def file, expect only adjacent same-row keys")
)

func main() {
	flag.Usage = func() {
		fmt.Fprintf(flag.CommandLine.Output(), `%[1]v generates SVG for a provided keymap.c file

If no keymap is provided, content is read from stdin until EOF is read.
The SVG is always written to stdout.

Usage:

	%[1]v [OPTIONS] [path/to/keymap.c]      Output SVG.
`, os.Args[0])
		flag.PrintDefaults()
	}

	flag.Parse()

	var keymap io.Reader

	switch len(flag.Args()) {
	case 0:
		keymap = os.Stdin
	case 1:
		file, err := os.Open(flag.Args()[0])
		if err != nil {
			fmt.Fprintf(os.Stderr, "failed opening %q with: %v\n", os.Args[1], err)
			os.Exit(1)
		}
		keymap = file
	default:
		flag.Usage()
		return
	}

	var comboContent []byte
	if *comboFile != "" {
		var err error
		comboContent, err = os.ReadFile(*comboFile)
		if err != nil {
			fmt.Fprintf(os.Stderr, "failed reading combos file: %v\n", err)
			os.Exit(1)
		}
	}

	content, err := ioutil.ReadAll(keymap)
	if err != nil {
		fmt.Fprintf(os.Stderr, "failed reading keymap: %v\n", err)
		os.Exit(1)
	}

	layouts, err := parseKeys(string(content), string(comboContent))
	if err != nil {
		fmt.Fprintf(os.Stderr, "failed parsing keys: %v\n", err)
	}
	if err := layouts.Render(os.Stdout); err != nil {
		fmt.Fprintf(os.Stderr, "failed rendering keymap %q: %v\n", keymap, err)
		os.Exit(1)
	}
}

const (
	keyDim   = 100
	keySpace = 10
)

type parsedLayout struct {
	name string
	keys [][]string
	// Left key index (row, col) -> combo-output keycode
	combos map[int]map[int]string
}

func (pl *parsedLayout) Width() int {
	return len(pl.keys[0])*(keyDim+keySpace) + (5 * keySpace)
}

func (pl *parsedLayout) Height() int {
	return len(pl.keys)*keyDim + (len(pl.keys)+1)*keySpace
}

var symbolize = map[string]string{
	"DRG_TOG": "SCROLL",
	"UNDO":    "↶",
	"REDO":    "↷",
	"ALT":     "⎇",
	"AMPR":    "&",
	"ASTR":    "*",
	"AT":      "@",
	"BSLS":    "\\",
	"BSPC":    "⇤",
	"BTN1":    "▐▌ │",
	"BTN2":    "│ ▐▌",
	"BTN3":    "│▐▌│",
	"CIRC":    "^",
	"COLON":   ":",
	"COMM":    ",",
	"COMMA":   ",",
	"CTL":     "⎈",
	"DOLLAR":  "$",
	"DOT":     ".",
	"DOWN":    "↓",
	"DPI":     "TRACK",
	"ENT":     "⤷",
	"ENTER":   "⤷",
	"EQUAL":   "=",
	"EXLM":    "!",
	"GRAVE":   "`",
	"GT":      ">",
	"GUI":     "🗔",
	"COPY":    "🗗",
	"CUT":     "✂",
	"PASTE":   "📋",
	"HASH":    "#",
	"LBRC":    "[",
	"LCBR":    "{",
	"LEFT":    "←",
	"LLCK":    "🔒",
	"LPRN":    "(",
	"LT":      "<",
	"MINS":    "-",
	"MINUS":   "-",
	"PERC":    "%",
	"PIPE":    "|",
	"PLUS":    "+",
	"QUOTE":   "'",
	"RBRC":    "]",
	"RCBR":    "}",
	"RIGHT":   "→",
	"RPRN":    ")",
	"SCLN":    ";",
	"SCR":     "SCROLL",
	"SFT":     "⇧",
	"SKYSFT":  "⇧",
	"SLASH":   "/",
	"SNP":     "SNIPE",
	"SPACE":   "‿",
	"TAB":     "⇥",
	"TILDE":   "~",
	"UNDS":    "_",
	"UP":      "↑",
}

func renderKey(canvas *svg.SVG, x, y int, key string) {
	var (
		keyColor  = "hsl(0, 0%, 90%)"
		textColor = "hsl(0, 0%, 10%)"
		keyText   = key
		underText = ""
	)
	switch key {
	case "XX":
		keyColor = "hsl(0, 0%, 20%)"
		keyText = ""
	case "__":
		keyColor = "hsl(0, 0%, 60%)"
		textColor = "hsl(0, 0%, 20%)"
		keyText = "[^]"
	case "os_GUI", "os_SFT", "os_ALT", "os_CTL":
		keyColor = "hsl(0, 30%, 80%)"
		keyText = strings.TrimPrefix(key, "os_")
		underText = "(oneshot)"
	case "SKYSFT":
		keyColor = "hsl(0, 30%, 80%)"
		keyText = "SFT"
		underText = "(sticky)"
	case "LLCK":
		keyColor = "hsl(80, 30%, 80%)"
		underText = "(layer)"
	case "REPEAT":
		keyColor = "hsl(180, 30%, 80%)"
		underText = "(last key)"
	case "DELIMS":
		keyColor = "hsl(180, 30%, 80%)"
		keyText = "*CASE"
		underText = "(toggle)"
	case "cy_DPI", "cy_SNP":
		keyColor = "hsl(220, 30%, 80%)"
		keyText = strings.TrimPrefix(key, "cy_")
		underText = "(dpi cycle)"
	case "ho_SNP", "ho_SCR":
		keyColor = "hsl(220, 30%, 80%)"
		keyText = strings.TrimPrefix(key, "ho_")
		underText = "(hold)"
	case "tg_SNP", "tg_SCR":
		keyColor = "hsl(220, 30%, 80%)"
		keyText = strings.TrimPrefix(key, "tg_")
		underText = "(toggle)"
	case "BTN1", "BTN2", "BTN3":
		keyColor = "hsl(290, 30%, 80%)"
		underText = "(mouse)"
	}
	if oneshotLayer := strings.TrimPrefix(keyText, "os"); oneshotLayer != keyText {
		keyColor = "hsl(80, 30%, 80%)"
		keyText = oneshotLayer
		underText = "(oneshot)"
	}
	if trackTap := strings.TrimPrefix(keyText, "trk_"); trackTap != keyText {
		keyColor = "hsl(30, 30%, 80%)"
		keyText = trackTap
		underText = "_TRACK"
	}
	if symbol, ok := symbolize[keyText]; ok {
		keyText = symbol
	}
	canvas.Roundrect(x, y, keyDim, keyDim, keySpace, keySpace, fmt.Sprintf("fill:%s", keyColor))
	if underText != "" {
		canvas.Text(x+(keyDim/2), y+(keyDim/2)+30, underText, fmt.Sprintf("font-size:10pt;fill:%s", textColor))
	}

	isASCII := true
	for i := 0; i < len(keyText); i++ {
		if keyText[i] > unicode.MaxASCII {
			isASCII = false
		}
	}
	if !isASCII {
		textColor += ";font-family:'sans-serif'"
	}
	canvas.Text(x+(keyDim/2), y+(keyDim/2)+10, keyText, fmt.Sprintf("font-size:20pt;fill:%s", textColor))
}

func (pl *parsedLayout) Render(canvas *svg.SVG) error {
	canvas.Text(
		2*keySpace,
		(len(pl.keys))*(keySpace+keyDim)-keySpace,
		"_"+pl.name,
		"text-anchor:start;font-size:30pt;fill:hsl(0, 0%, 90%)",
	)
	standardRow := len(pl.keys[0])

	for y, row := range pl.keys {
		combos, ok := pl.combos[y]
		if !ok {
			combos = map[int]string{}
		}
		for x, key := range row {
			if len(row) < standardRow {
				x += (standardRow - len(row)) / 2
			}

			left := x*(keySpace+keyDim) + keySpace
			if x >= (standardRow / 2) {
				left += 3 * keySpace
			}
			top := y*(keySpace+keyDim) + keySpace
			renderKey(canvas, left, top, key)

			if comboKey, ok := combos[x]; ok {
				x, y := left-((keyDim+keySpace)/2), top
				if symbol, ok := symbolize[comboKey]; ok {
					comboKey = symbol
				}
				canvas.Roundrect(x+(keyDim/6), y+keyDim/3, 2*keyDim/3, keyDim/3, keySpace, keySpace, "fill:hsl(50, 80%, 50%)")
				isASCII := true
				for i := 0; i < len(comboKey); i++ {
					if comboKey[i] > unicode.MaxASCII {
						isASCII = false
					}
				}
				textColor := "fill:hsl(0, 0%, 10%)"
				if !isASCII {
					textColor += ";font-family:'sans-serif'"
				}
				canvas.Text(x+(keyDim/2), y+(keyDim/2)+5, comboKey, fmt.Sprintf("font-size:10pt;fill:%s", textColor))
			}
		}
	}
	return nil
}

type parsedKeyboard []parsedLayout

func (pk parsedKeyboard) Render(w io.Writer) error {
	canvas := svg.New(w)
	canvas.Start(pk[0].Width()+(2*keySpace), len(pk)*(pk[0].Height()+keySpace)+keySpace)
	canvas.Group("font-family:'monospace';text-anchor:middle")
	for i, layout := range pk {
		canvas.Translate(keySpace, (i*layout.Height())+((i+1)*keySpace))
		fill := fmt.Sprintf("fill:hsl(%d, 15%%, 30%%)", (360/(len(pk)))*i)
		canvas.Roundrect(0, 0, layout.Width(), layout.Height(), keySpace, keySpace, fill)
		if err := layout.Render(canvas); err != nil {
			return fmt.Errorf("rendering layout %q: %w", layout.name, err)
		}
		canvas.Gend()
	}
	canvas.Gend()
	canvas.End()
	return nil
}

func parseKeys(keymap string, combos string) (parsedKeyboard, error) {
	var layouts parsedKeyboard

	var (
		inLayout bool
		current  *parsedLayout
	)
	for _, line := range strings.Split(keymap, "\n") {
		switch {
		case strings.HasPrefix(line, "#define _"):
			current = &parsedLayout{combos: map[int]map[int]string{}}
			current.name = strings.TrimPrefix(line, "#define _")
			current.name = strings.SplitN(current.name, " ", 2)[0]
			inLayout = true
		case inLayout && line == "":
			layouts = append(layouts, *current)
			inLayout = false
		case inLayout && strings.Trim(line, ` /*-+|\`) != "":
			line = strings.ReplaceAll(line, " ", "")
			line = strings.ReplaceAll(line, `,\`, "")
			current.keys = append(current.keys, strings.Split(line, ","))
		}
	}

	parsedCombos := map[string]map[string]string{}
	for _, line := range strings.Split(combos, "\n") {
		if len(line) == 0 {
			continue
		}

		var (
			left, right, output string
		)
		for i, key := range strings.Split(line, ",") {
			key = strings.TrimSuffix(strings.TrimPrefix(strings.TrimSpace(key), "KC_"), ")")
			switch i {
			case 0:
				// Do nothing.
			case 1:
				output = key
			case 2:
				left = key
			case 3:
				right = key
			default:
				return nil, fmt.Errorf("invalid combos file, only keypairs allowed, line: %q", line)
			}
		}

		createKey := func(one, two string) {
			if _, ok := parsedCombos[one]; !ok {
				parsedCombos[one] = map[string]string{}
			}
			parsedCombos[one][two] = output
		}
		createKey(left, right)
		createKey(right, left)
	}

	for _, layout := range layouts {
		for row, rowKeys := range layout.keys {
			for col, right := range rowKeys {
				if col == 0 {
					continue
				}
				if second, ok := parsedCombos[right]; ok {
					left := rowKeys[col-1]
					if output, ok := second[left]; ok {
						if _, ok := layout.combos[row]; !ok {
							layout.combos[row] = map[int]string{}
						}
						layout.combos[row][col] = output

						delete(parsedCombos[left], right)
						if len(parsedCombos[left]) == 0 {
							delete(parsedCombos, left)
						}
						delete(parsedCombos[right], left)
						if len(parsedCombos[right]) == 0 {
							delete(parsedCombos, right)
						}
					}
				}
			}
		}
	}

	if len(parsedCombos) > 0 {
		return nil, fmt.Errorf("keypairs must be adjacent, unhandled pairs: %+v", parsedCombos)
	}

	return layouts, nil
}
