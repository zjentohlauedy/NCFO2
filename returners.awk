BEGIN { kr = "false"; }
$1 == "Returners" { kr = "true"; }
$1 ~ /[QRWTODLCSKP][BREL ]/ {
  if (kr == "false") {
    print $0;
  } else {
    split($0, flds, FS, seps);
    flds[1] = "KR";
    for (i = 1; i in flds; i++)
      printf "%s%s", flds[i], seps[i]
    print ""
  }
}
$1 ~ /[A-Z][a-z]+/ && $1 != "Returners" { kr = "false"; }
$1 !~ /[QRWTODLCSKP][BREL ]/ { print $0; }
