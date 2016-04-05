# Practical Performance Practices


# Jason Turner

*about me*

# Background

> - ChaiScript has been in development since 2009
> - Version 5.8 is over 100x faster than 1.0
> - Performance measuring is difficult
>     * Great number of template instantations
>     * Nature of scripting means execution is spread over many functions

# Performance Practices

This lead to the creation of several rules and practices that I follow to make well-performing code 'by default'

# Don't Do More Work Than You Have To


# Don't Do More Work Than You Have To

## Always `const`

```cpp
int main()
{
  std::string s;
  s = "A Somewhat Rather Long String";
}
```

> - Construct a string object
> - Reassign string object


# Don't Do More Work Than You Have To

## Always `const`

```cpp
int main()
{
  const std::string s = "A Somewhat Rather Long String";
}
```

> - Construct and initialize in one step
> - ~32% more efficient


# Don't Do More Work Than You Have To

## Always `const` - Complex Initialization

```cpp
int main()
{
  std::string s;
  switch (i%4) {
    case 0:
      s = "long string is mod 0";
      break;
    case 1:
      s = "long string is mod 1";
      break;
    case 2:
      s = "long string is mod 2";
      break;
    case 3:
      s = "long string is mod 3";
      break;
  }
}
```

# Don't Do More Work Than You Have To

## Always `const` - Complex Initialization - Using IIFE

```cpp
int main()
{
  const std::string s = [&](){
    switch (i%4) {
      case 0:
        return "long string is mod 0";
      case 1:
        return "long string is mod 1";
      case 2:
        return "long string is mod 2";
      case 3:
        return "long string is mod 3";
    }
  }();
}
```

 - ~31% more efficient
 
# Don't Do More Work Than You Have To

## Always Initialize When Const Isn't Practical

```cpp
struct Int
{
  Int(std::string t_s)
  {
    m_s = t_s;
  }

  int eval() const {
    return std::atoi(m_s.c_str());
  }

  std::string m_s;
};
```

# Don't Do More Work Than You Have To

## Always Initialize When Const Isn't Practical

```cpp
struct Int
{
  Int(std::string t_s) : m_s(std::move(t_s))
  {
  }

  int eval() const {
    return std::atoi(m_s.c_str());
  }

  std::string m_s;
};
```

 - Same gains as const initializer
 
# Don't Do More Work Than You Have To

## Don't Recalculate Values

```cpp
struct Int
{
  Int(std::string t_s) : m_s(std::move(t_s))
  { }

  int eval() const {
    return std::atoi(m_s.c_str());
  }

  std::string m_s;
};
```


# Don't Do More Work Than You Have To

## Don't Recalculate Values

```cpp
struct Int
{
  Int(const std::string &t_s) : m_i(std::atoi(t_s))
  { 
  }

  int eval() const {
    return m_i;
  }

  int m_i;
};
```

> - In the context of a large code base, this took ~2 years to find
> - Resulted in 45% performance improvement across system







