#!/bin/bash

IP=${1:-"192.168.1.100"}
PORT=${2:-80}
BASE="http://$IP:$PORT"

ICON="ABgAAZmAAf+AGf+YH//4DgAwDv/wfH/+fv+uP/UEP1VU8VF39xV/MVX8PV/8cf++f/8eD/+wDgAwH//4Gf+YAf+AAZmAABgA"
ICON_W=24
ICON_H=24

pass=0
fail=0

run_test() {
  local description=$1
  local data=$2
  echo ""
  echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
  echo "TEST: $description"
  echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

  status=$(curl -s -o /dev/null -w "%{http_code}" "$BASE" --data "$data")

  if [ "$status" == "200" ]; then
    echo "HTTP: $status OK"
  else
    echo "HTTP: $status FAILED"
    ((fail++))
    return
  fi

  read -p "PASS or FAIL? [P/f]: " result
  if [ "$result" == "f" ] || [ "$result" == "F" ]; then
    echo "FAILED"
    ((fail++))
  else
    echo "PASSED"
    ((pass++))
  fi
}

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "Train Signs Integration Test Suite"
echo "Target: $BASE"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# ── Text alignment (9 cases) ──────────────────────────────────────────────────

run_test "Text: left + top" '{
  "text": "LT",
  "horizontal_align": "left",
  "vertical_align": "top"
}'

run_test "Text: center + top" '{
  "text": "CT",
  "horizontal_align": "center",
  "vertical_align": "top"
}'

run_test "Text: right + top" '{
  "text": "RT",
  "horizontal_align": "right",
  "vertical_align": "top"
}'

run_test "Text: left + middle" '{
  "text": "LM",
  "horizontal_align": "left",
  "vertical_align": "middle"
}'

run_test "Text: center + middle" '{
  "text": "CM",
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

run_test "Text: right + middle" '{
  "text": "RM",
  "horizontal_align": "right",
  "vertical_align": "middle"
}'

run_test "Text: left + bottom" '{
  "text": "LB",
  "horizontal_align": "left",
  "vertical_align": "bottom"
}'

run_test "Text: center + bottom" '{
  "text": "CB",
  "horizontal_align": "center",
  "vertical_align": "bottom"
}'

run_test "Text: right + bottom" '{
  "text": "RB",
  "horizontal_align": "right",
  "vertical_align": "bottom"
}'

# ── Image alignment (9 cases) ─────────────────────────────────────────────────

run_test "Image: left + top" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "left",
  "vertical_align": "top"
}'

run_test "Image: center + top" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "center",
  "vertical_align": "top"
}'

run_test "Image: right + top" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "right",
  "vertical_align": "top"
}'

run_test "Image: left + middle" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "left",
  "vertical_align": "middle"
}'

run_test "Image: center + middle" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

run_test "Image: right + middle" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "right",
  "vertical_align": "middle"
}'

run_test "Image: left + bottom" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "left",
  "vertical_align": "bottom"
}'

run_test "Image: center + bottom" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "center",
  "vertical_align": "bottom"
}'

run_test "Image: right + bottom" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "right",
  "vertical_align": "bottom"
}'

# ── Scrolling ─────────────────────────────────────────────────────────────────

run_test "Scroll: horizontal" '{
  "speed": 50,
  "text": "This is a long horizontally scrolling message for Sheffield Hackspace",
  "text_wrap": false
}'

run_test "Scroll: vertical" '{
  "speed": 50,
  "text": "Line one\nLine two\nLine three\nLine four\nLine five\nLine six\nLine seven\nLine eight\nLine nine\nLine ten\nLine eleven\nLine twelve\nLine thirteen\nLine fourteen",
  "text_wrap": true
}'

# ── Fonts ─────────────────────────────────────────────────────────────────────

run_test "Font: small" '{
  "text": "Small font",
  "text_size": 1,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

run_test "Font: medium" '{
  "text": "Medium font",
  "text_size": 2,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

run_test "Font: large" '{
  "text": "Large font",
  "text_size": 3,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

# ── Flashing ──────────────────────────────────────────────────────────────────

run_test "Flashing: on" '{
  "text": "Flash!",
  "flashing": true,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

run_test "Flashing: off" '{
  "text": "No flash",
  "flashing": false,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

# ── Inverted ──────────────────────────────────────────────────────────────────

run_test "Inverted: on" '{
  "text": "Inverted",
  "inverted": true,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

run_test "Inverted: off" '{
  "text": "Normal",
  "inverted": false,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

# ── Image + text ──────────────────────────────────────────────────────────────

run_test "Image + text: left aligned, no wrap" '{
  "text": "Hackspace",
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "text_wrap": false,
  "horizontal_align": "left",
  "vertical_align": "middle"
}'

run_test "Image + text: center aligned, no wrap" '{
  "text": "Hackspace",
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "text_wrap": false,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

run_test "Image + text: right aligned, no wrap" '{
  "text": "Hackspace",
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "text_wrap": false,
  "horizontal_align": "right",
  "vertical_align": "middle"
}'

run_test "Image + text: left aligned, wrap" '{
  "text": "Sheffield\nHackspace",
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "text_wrap": true,
  "horizontal_align": "left",
  "vertical_align": "middle"
}'

run_test "Image + text: center aligned, wrap" '{
  "text": "Sheffield\nHackspace",
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "text_wrap": true,
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

run_test "Image + text: right aligned, wrap" '{
  "text": "Sheffield\nHackspace",
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "text_wrap": true,
  "horizontal_align": "right",
  "vertical_align": "middle"
}'

# ── Image only ────────────────────────────────────────────────────────────────

run_test "Image only" '{
  "image": "'"$ICON"'",
  "image_width": '"$ICON_W"',
  "image_height": '"$ICON_H"',
  "horizontal_align": "center",
  "vertical_align": "middle"
}'

# ── Clear display ─────────────────────────────────────────────────────────────

run_test "Clear display" '{
  "text": "",
  "image": ""
}'

# ── Summary ───────────────────────────────────────────────────────────────────

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "Results: $pass passed, $fail failed"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
