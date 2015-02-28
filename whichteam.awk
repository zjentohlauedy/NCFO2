BEGIN { team = "No Team" }
$0 ~ /^[A-Z][a-z]+/ { team = $0; }
$3 ~ player { printf "%s %s: %s\n", $3, $4, team }
