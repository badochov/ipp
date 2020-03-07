#!/bin/bash
name=$1
dir=$2

total=0
correct=0
leaked=0
function setval { printf -v "$1" "%s" "$(cat)"; declare -p "$1"; }

for f in "$dir"/*.in; do
  echo -e "\e[1mTest $f \e[0m"

  sto=$(./"$name" <"$f" 2>/dev/null)
  ste=$(./"$name" <"$f" 2>&1 1>/dev/null)
  err=$?


  if [[ $err != 0 ]]; then
    echo -e "\e[1;31m\tProgram zakończył się kodem $err\e[0m"
  fi

  d_out=$(diff "${f%in}"out <(echo "$sto"))
  d_err=$(diff "${f%in}"err <(echo "$ste"))
  if [[ $d_out != "" ]] || [[ $d_err != "" ]]; then
    echo -e "\e[1;31m\tZły wynik testu\e[0m"
  else
    echo -e "\e[1;32m\tPoprawny wynik testu\e[0m"
    ((correct++))
  fi
  ((total++))

  valgrind --leak-check=full --error-exitcode=1 ./"$name" command <"$f" >/dev/null 2>&1
  err=$?

  if [[ $err != 0 ]]; then
    echo -e "\e[1;31m\tWyciek pamięci\e[0m"
  else
    echo -e "\e[1;32m\tBrak wycieku pamięci\e[0m"
    ((not_leaked++))
  fi

  echo ""
done

echo -e "Poprawne \e[1m$correct\e[0m na \e[1m$total\e[0m testów"

echo -e "Wyciekła pamięć w \e[1m$leaked\e[0m na \e[1m$total\e[0m testów"

if [[ $leaked == 0 ]] && [[ $correct == "$total" ]]; then
  echo -e "\e[1;92mWszystko dobrze! \e[0m"
fi
