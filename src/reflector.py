#!/usr/bin/env python3.8
# -*- coding: utf-8 -*-

import click
import re
import jinja2
import os

LINE_REGEX = re.compile("^enum class Opcode \{(.*?)\};$")

def make_env() -> jinja2.Environment:
    loader = jinja2.FileSystemLoader(searchpath="/")
    return jinja2.Environment(loader=loader)

env = make_env()

@click.command()
@click.argument("name")
@click.argument("out_file")
def reflect_tokens(name: str, out_file: str) -> None:
    in_file = f"{name}.h"
    template_file = f"{name}.template.cpp"
    with open(in_file, "r") as inf:
        while (line := inf.readline()) != "":
            if (match := re.match(LINE_REGEX, line)) is not None:
                (value, ) = match.groups()
                values = (x.strip() for x in value.split(","))
                template = env.get_template(template_file)
                with open(out_file, "w") as of:
                    of.write(template.render(opcodes=values))
                return
        raise Exception("Could not find line in input file")

if __name__ == "__main__":
    reflect_tokens()
