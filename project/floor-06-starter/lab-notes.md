Some screwing around (replace on Friday commit):
> lint ()
  balanced
> lint )(
  not balanced
> lint (())
  balanced
> lint ({)}
  not balanced
> lint ({})
  balanced
> lint for (int i = 0; i < getv({6, 2}, new v[5]); ++i) {dosmth();}
  balanced
> lint ][{}
  not balanced
> lint (((((((({{{{{{{{[[[[[[[[]]]]]]]]}}}}}}}}))))))))
  balanced
> lint <>
  balanced
> lint <()
  balanced
> lint a < 5 (reason why we don't test chevrons...?)
  balanced
> lint I finally have a way to return obnoxiousness to people who don't close their parentheses (like this.
  not balanced