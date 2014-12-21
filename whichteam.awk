BEGIN { found = 0; team = "No Team" }
$0 ~ /^[A-Z][a-z]+/ { if ( found == 0 ) team = $0; }
$3 ~ player { found = 1; }
END { if ( found == 1 ) { print team; } else { print "Not Found" } }
