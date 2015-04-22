#ifndef INPUTTOOLS_H
#define INPUTTOOLS_H

char InputEquals( const char[] aText, int aTextLength, const char[] aInput )
{
    for ( int i = 0; i < aTextLength; i++ ) {
        if ( aText[i] != aInput[i] ) {
            return 0;
        }
    }
    return 1;
}

#endif // INPUTTOOLS_H

