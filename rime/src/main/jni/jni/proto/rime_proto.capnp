@0xde912f558dde6b99;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("rime::proto");

struct Commit {
  # Text to commit to input field.
  text @0 :Text;
}

struct Candidate {
  text @0 :Text;
  comment @1 :Text;
  label @2 :Text;
}

struct Context {
  # Input context.

  struct Composition {
    # (de-)Compostion of current input.
    length @0 :Int32;
    cursor @1 :Int32;
    selHead @2 :Int32;
    selTail @3 :Int32;
    preedit @4 :Text;
    preview @5 :Text;
  }

  struct Menu {
    # Menu of text candidates.
    pageSize @0 :Int32;
    pageNum @1 :Int32;
    lastPage @2 :Bool;
    highlight @3 :Int32;
    candidates @4 :List(Candidate);
    selKeys @5 :Text;
    selLabels @6 :List(Text);
  }

  composition @0 :Composition;
  menu @1 :Menu;
  input @2 :Text;
  caret @3 :Int32;
}

struct Status {
  schemaId @0 :Text;
  schemaName @1 :Text;
  disabled @2 :Bool;
  composing @3 :Bool;
  asciiMode @4 :Bool;
  fullShape @5 :Bool;
  simplified @6 :Bool;
  traditional @7 :Bool;
  asciiPunct @8 :Bool;
}
