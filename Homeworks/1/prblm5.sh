i=1;
lines=0;
temp=0;
rm -f totalLines.txt
for file in "$@" 
do
    temp=$(wc -l < $file);
    temp=$((temp + 1))
    echo "File $i: $file ($temp lines)";
    echo "File $i: $file ($temp lines)" >> totalLines.txt;
    lines=$((lines + temp));
    echo "Total Lines in $i files: $lines";
    echo "Total Lines in $i files: $lines" >> totalLines.txt;
    i=$((i + 1));
done
