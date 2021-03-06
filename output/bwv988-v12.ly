% Automatically generated by BMC, the braille music compiler
\version "2.14.2"
\include "articulate.ly"
music =
  <<
    \new PianoStaff <<
      \new Staff = "RH" {
        \clef "treble"
        \key g \major
        \time 3/4
        r8 g'16-3 fis' g'8-1 a'16 b' c''[ b' a' g'] | % 1
        d''16[ a' b' cis''] d''-1 e'' fis'' g'' a''8 r | % 2
        r8 g''16 fis'' e''[ d'' cis''-4 b'] a'[ g' fis'-2 g'] | % 3
        g'8-4( fis'16) e' d'8 r r4 | % 4
        << {r16 d''8.~ d''16[ b' a' g'] f'-3[ e' d' f']}\\{d'8-1( e'16-1) fis' g'8-1 r r4} >> | % 5
        << {e'16 g'-1 c''-3 d'' e''8 a'4-5 a'8~-4}\\{r16  g8.~-2 g16 b  c'-1 d' e'-1[ fis' g' e']} >> | % 6
        << {a'16[ b' fis' g'-1] a'-3[ g' fis'-1 e''] d''[ c'' b' a']}\\{fis'16 d'  a g fis8 c'4 c'8~} >> | % 7
        << {g'4-4 r r}\\{ c'16  b e'-2 d' c'-3[ d' e' fis-2] g-1[ a b c']} >> | % 8
        << {r16[ fis'-2 g' a'] b'-1[ cis'' d'' fis''] e''[ d''-1 cis''-3 b']}\\{d'4-1 r r} >> | % 9
        << {a'16 g'-4 fis' e'-2 d'8-"1-5" r r4}\\{r16 e' d'  cis'-2 b-1  a g e-2 fis[ g-1 a b]} >> | % 10
        << {r8 cis''~ cis''16[ b'-3 a' b'] cis''[ d''-1 e''-3 fis'']}\\{cis'16 d'-1 e' fis' g'8-1 r r4} >> | % 11
        << {g''8 g'16 fis'-4 g'4~ g'16-"5-4"[ b' a' g'-1]}\\{r8  a~ a16  b-1 cis'-3 b a  g fis e-1} >> | % 12
        << {fis'16-3[ e' fis' g'] fis'[ a'-4 g'-5 fis'] e'4~-5}\\{ d8  d'16 e' d'4~ d'16[ b-2 cis' d'-1]} >> | % 13
        << {e'8-2 d'' cis''-4 b' a'-2 fis''}\\{e'16-2[ fis' e'-2 d'] e'  cis'-1 d'-1 e'-1  fis'4~-1} >> | % 14
        << {g'4.-"2-5" fis'8 e'16-3 a' g'8~}\\{fis'8  g-2 a-1  b-1 cis'-2  e-1} >> | % 15
        << {g'8 fis'16 e' fis' d''8 cis''16 d''8 r}\\{ d'4.-1 e'8-1 fis'16 cis' d'8 } >> \bar ":|:" % 16
         r8 d'16-4 e' d'8 c'16 b a[ b c' d'] | % 17
        r8 g'16-3 fis' g'8-1 a'16 b' c''[ b' a' g'] | % 18
        g''4~ g''16[ fis''-3 a'' g''] fis'' g'' e''8 | % 19
        \appoggiatura e''16 fis''8~ fis''32 eis'' fis'' g'' fis''8 a' r fis''-3 | % 20
        g''16[ fis'' g'' a''] g''[ fis'' e'' d''] c''-4[ b' a' g'] | % 21
        << {fis'2.~-4}\\{ e'8  e'16 dis' e'8 e' e' dis'16 cis'} >> | % 22
        << {fis'8 fis'16 g' fis'8 fis' fis'-3 g'16 a'}\\{dis'4 r fis'~-3} >> | % 23
        << {g'4 r e'~-5}\\{fis'16[ e'-1 dis' fis'-3] e'-2[ d' cis'-3 c'] b-1 a-2  g fis-2} >> | % 24
        << {e'16[ fis'-4 g' e'-1] fis'[ g' gis' a'-1] b'[ c'' d'' e'']}\\{ g8 r r4 r} >> | % 25
        << {d''8-"4-5" r r4 r}\\{r16  a' gis'-3 fis' gis'[ f'-2 e' d'-3] e'[ d' c' b]} >> | % 26
        << {r16[ c'-1 d' e'] d'[ ees' fis' g'] fis'-2[ g' a' b']}\\{c'2.~} >> | % 27
        << {a'2.~-"4-5"}\\{c'8 g' fis' e'-1 d'16-2[ b c' a]} >> | % 28
        << {a'8 d'-3 e' fis' g'16-1[ b' a' c'']}\\{b8-2 c'-1 b  a-1 g8. fis16} >> | % 29
        b'8 a'-1 b' c'' d''8.\mordent  e''16 | % 30
        d''4~ d''16[ b'-1 c'' d''] e''-1[ fis'' g'' a''] | % 31
        b''16 g'' d'' b' g'8-2 r r4  \bar ":|" % 32
      }
      \new Staff = "LH" {
        \clef "bass"
        \key g \major
        \time 3/4
        g4-4 g g | % 1
        << {fis4 fis fis}\\{r8 d'16-2 e' d'8 c'16-1 b-3 a-4[ b c' d']} >> | % 2
        << {e4 e e}\\{g16-4[ c' b a-1] g fis e-1 d-3 cis8 r} >> | % 3
        << {d16 d, d8 r c16-3 b, a,8 c}\\{r8 d16 e-3 fis g  a-2 b c'-1[ d' e' d']} >> | % 4
        b,4 b, b,-4 | % 5
        c4-5 c c | % 6
        d4 d d | % 7
        g8. b,16 e-2[ d c e] d-1[ c b, a,] | % 8
        b,8 g, r g16-2 fis g8 e | % 9
        fis8 b r b,16-3 cis d8 b,-4 | % 10
        e16[ fis-2 g fis] e8 fis e-1 d-3 | % 11
        cis16 d e d cis8 e-2 cis a, | % 12
        d4 r16[ d-5 e fis] g[ a g fis] | % 13
        g8-1 g, r16[ e-5 fis-4 g-3] a[ fis-3 e d] | % 14
        b16[ b, e-3 d-5] cis-4[ a, d-3 g-2] a[ e-2 cis-3 a,] | % 15
        d16 e d cis d8-1 a, d, r  \bar ":|:" % 16
         d4-3 d c | % 17
        << {b,8 c b, a,-4 g, a,16 b,-5}\\{d4~ d16[ e-1 c-3 d] e d f8} >> | % 18
        << {c8 b, c e-2 a, c-3}\\{\appoggiatura f16 e8~ e32 f e dis e8 c' r e} >> | % 19
        << {b,4 r r}\\{dis16[ e dis cis] dis[ e fis-4 g] a[ b cis'-3 dis']} >> | % 20
        << {r8 e fis g-5 a b-"3-4"}\\{e'2.~-4} >> | % 21
        c'16[ d' c' b] c'-1[ b a g] a-1[ fis-4 g a] | % 22
        b16[ c'-2 b a] b-1[ fis dis fis] b,[ dis cis b,] | % 23
        e16-1[ c-2 b, a,] g, b, e dis-4 e8 d-4 | % 24
        c8 c' r16[ e-2 d c] d-1[ c b, a,] | % 25
        b,8-5 b r16[ d-2 c b,] c-1[ b, a, gis,] | % 26
        a,8-5 a r16[ b-2 a g] a-1[ g fis e] | % 27
        g16[ e-1 d cis] d-1[ c b, a,] b,-1[ a, g, fis,] | % 28
        g,16-5[ a g fis] g[ fis e-4 dis] e4~ | % 29
        << {e16-5[ d-4 c e] d4.-5 a,8-5}\\{g4~-1 g16[ b-2 a-1 g] fis[ e-1 d c]} >> | % 30
        << {b,8 r r e c d-3}\\{b,16 d g b-2 d'8 g~ g16 a fis8-2} >> | % 31
        << {g,4~ g,16[ b, d-3 fis] g4}\\{g4 r r } >> \bar ":|" % 32
      }
    >>
  >>

\score {
  \music
  \layout { }
}
\score {
  \unfoldRepeats \articulate \music
  \midi { }
}
