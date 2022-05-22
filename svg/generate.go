package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"strings"
	"unicode"

	svg "github.com/ajstarks/svgo"
)

var help = `%[1]v generates SVG for a provided keymap.c file

Usage:

	%[1]v [path/to/keymap.c]      Output SVG.
	%[1]v --help                  This content.

If no keymap is provided, content is read from stdin until EOF is read.
The SVG is always written to stdout.
`

func main() {
	var keymap io.Reader

	switch len(os.Args) {
	case 1:
		keymap = os.Stdin
	case 2:
		if os.Args[1] == "--help" || os.Args[1] == "-h" {
			fmt.Printf(help, os.Args[0])
			return
		}

		file, err := os.Open(os.Args[1])
		if err != nil {
			fmt.Fprintf(os.Stderr, "failed opening %q with: %w\n", os.Args[1], err)
			os.Exit(1)
		}
		keymap = file
	default:
		fmt.Printf(help, os.Args[0])
		return
	}

	content, err := ioutil.ReadAll(keymap)
	if err != nil {
		fmt.Fprintf(os.Stderr, "failed reading keymap: %w", err)
		os.Exit(1)
	}

	if err := parseKeys(string(content)).Render(os.Stdout); err != nil {
		fmt.Fprintf(os.Stderr, "failed rendering keymap %q: %w\n", keymap, err)
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
}

func (pl *parsedLayout) Width() int {
	return len(pl.keys[0])*(keyDim+keySpace) + (5 * keySpace)
}

func (pl *parsedLayout) Height() int {
	return len(pl.keys)*keyDim + (len(pl.keys)+1)*keySpace
}

var symbolize = map[string]string{
	"UNDO":   "↶",
	"REDO":   "↷",
	"ALT":    "⎇",
	"AMPR":   "&",
	"ASTR":   "*",
	"AT":     "@",
	"BSLS":   "\\",
	"BSPC":   "⇤",
	"BTN1":   "▐▌ │",
	"BTN2":   "│ ▐▌",
	"BTN3":   "│▐▌│",
	"CIRC":   "^",
	"COLON":  ":",
	"COMM":   ",",
	"COMMA":  ",",
	"CTL":    "⎈",
	"DOLLAR": "$",
	"DOT":    ".",
	"DOWN":   "↓",
	"DPI":    "TRACK",
	"ENT":    "⤷",
	"ENTER":  "⤷",
	"EQUAL":  "=",
	"EXLM":   "!",
	"GRAVE":  "`",
	"GT":     ">",
	"GUI":    "🗔",
	"COPY":   "🗗",
	"CUT":    "✂",
	"PASTE":  "📋",
	"HASH":   "#",
	"LBRC":   "[",
	"LCBR":   "{",
	"LEFT":   "←",
	"LLCK":   "🔒",
	"LPRN":   "(",
	"LT":     "<",
	"MINS":   "-",
	"MINUS":  "-",
	"PERC":   "%",
	"PIPE":   "|",
	"PLUS":   "+",
	"QUOTE":  "'",
	"RBRC":   "]",
	"RCBR":   "}",
	"RIGHT":  "→",
	"RPRN":   ")",
	"SCLN":   ";",
	"SCR":    "SCROLL",
	"SFT":    "⇧",
	"SKYSFT": "⇧",
	"SLASH":  "/",
	"SNP":    "SNIPE",
	"SPACE":  "‿",
	"TAB":    "⇥",
	"TILDE":  "~",
	"UNDS":   "_",
	"UP":     "↑",
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

func parseKeys(keymap string) parsedKeyboard {
	var layouts parsedKeyboard

	var (
		inLayout bool
		current  *parsedLayout
	)
	for _, line := range strings.Split(keymap, "\n") {
		switch {
		case strings.HasPrefix(line, "#define _"):
			current = &parsedLayout{}
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

	return layouts
}
