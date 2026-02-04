# Xerith

Xerith is a personal programming language project I’m building to learn more about how languages actually work under the hood. The main goal isn’t to compete with existing languages, but to design and implement something small, opinionated, and understandable from end to end.

This repo is where I experiment with syntax, parsing, execution, and tooling, and where I document what works and what doesn’t.

## Trademark Notice

The name **“Xerith”** is a registered trademark of NerdBlud.  
You may fork, modify, and redistribute the **code** under the AGPL v3 license, but you **may not redistribute it under the name “Xerith”** or any confusingly similar name without explicit permission.  

## Why I’m building this

I want to go beyond just using programming languages and actually understand:

* How lexers and parsers turn text into structures
* How a language runtime executes code
* How type systems (or no type systems) affect design
* What makes a language pleasant or painful to use
* How tooling (CLI, errors, formatting) changes the developer experience

Xerith is basically my playground for all of that.

## Project goals

Some things I’m aiming for:

* A small, readable core language
* Simple syntax that doesn’t try to be clever
* Clear error messages (even if basic at first)
* Fast iteration while experimenting with features
* A codebase that I can actually understand later

## Non-goals (for now)

Things I’m intentionally not focusing on:

* Being faster than existing languages
* Being production-ready
* Having a huge standard library
* Supporting every platform perfectly
* Fancy IDE integrations

If it works and teaches me something, that’s good enough.

## What I plan to implement

Rough roadmap (will change):

* Basic lexer and parser
* AST representation
* Simple interpreter or bytecode VM
* Variables and basic types
* Functions
* Control flow (if, while, etc.)
* Basic error reporting
* A small CLI to run `.xrtx` files

## What I want to learn from this

By the end of this project, I want to be able to:

* Read other language implementations and understand them
* Debug parsing and runtime issues more confidently
* Make better design decisions when building tools
* Have a clearer mental model of how languages actually execute code

## Current state

This project is early-stage and unstable. Things will break. Syntax will change. Files might get rewritten. That’s expected.

If you’re looking for something stable, this isn’t it. If you’re curious about language internals, this might be interesting to follow.

## Notes to future me

Don’t over-engineer this. Keep it simple. Get something running, even if it’s ugly. Clean design can come later.
