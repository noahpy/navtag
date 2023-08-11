
function mark(){
    navtag $mfp -a $1 $(pwd)
}

function unmark(){
    navtag $mfp -d $1
}

function marks(){
    navtag $mfp -l
}

function cdx(){
    cd $(navtag $mfp -t $@)
}

function mvx(){
    mv $(navtag $mfp -t $@)
}

function cpx(){
    cp $(navtag $mfp -t $@)
}

function mkx(){
    mkdir $(navtag $mfp -t $@)
}

function mcx(){
    results=$(navtag $mfp -t $@)
    mkdir $results
    cd $results
}

