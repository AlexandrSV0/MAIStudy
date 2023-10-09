let test env = 
    let test_impl source =
        try
            printfn "Source: %s" source
            let tokens = tokenize ("(" + source + ")" |> Seq.toList)
            let expr = parse tokens
            let evaluated, new_env = evaluate  env expr
            printfn "Processed: %A\n" evaluated
            ()
        with ex ->
            printfn "Exception: %s\n" ex.Message
    test_impl "(1)"
    test_impl "(\"string\")"
    test_impl "(+ 1 2)"
    test_impl "(+ 1.2 2.4)"
    test_impl "(1)"
    test_impl "(\"string\")"
    test_impl "(var id 1)"
    test_impl "((var id 1) (+ id 1))"
    test_impl "((var id ( + 1 2 ) ) id)"
    test_impl "(if (1) then (2) else (3))"
    test_impl "(var id (if 1 then 2 else 3))"
    test_impl "((var id 1) (+ id 1))"
    test_impl "((var id_1 1) (var id_2 2) (+ id_1 id_2))"
    test_impl "(def id_1 {arg_1 arg_2 } ( + arg_1 arg_2 ))"
    test_impl "((def id_1 {arg_1 arg_2} ( + arg_1 arg_2 ) ) (id_1 1 (if 0 then 15 else 8 )))"
    test_impl "((def id_1 {arg_1 arg_2} ( + arg_1 arg_2 ) ) (var id_for_func 7) ( id_1 id_for_func (if 0 then 15 else 8)))"
    test_impl "((def id_1 {arg_1 arg_2} ( + arg_1 arg_2 ) ) (var arg_2 7) (id_1 2 1))"
    test_impl "((var closure_id 8 ) (def id_1 {arg_1 arg_2} (+ arg_1 arg_2 closure_id)) (var id_for_func 7) (id_1 id_for_func (if 0 then 15 else 8)))"
    test_impl "((def id_1 {arg_1} ( if ( = arg_1 1 ) then arg_1 else (* ( id_1 ( - arg_1 1 ) ) arg_1 ) ) ) ( id_1 5 ))"
    test_impl "((def id_1 {} (if true then 1 else false)) (id_1))"
    test_impl "((def id_1 {a} (if (> a 1) then ((sout a) (id_1 (- a 1))) else \"function ended\")) (id_1 5))"
    test_impl "(var id (+ 1 2)))"
    test_impl "(var id (+ 1 2)))"
    test_impl "(if (| true 1) then 1 else 2))"
    test_impl "(if (& true (< 1 0)) then 1 else 2)"
    test_impl "((var id 1) (def arg {} (+ id 1)) (arg))"
    test_impl "((var id 1) (sout id) (put id 2) (sout id))"
    test_impl "(+ 1 $ 1 2 3 $ 4)"
    test_impl "(
  (var closure_id 8 ) 
  (def id_1 {arg_1 arg_2} (+ arg_1 arg_2 closure_id)) 
  (var id_for_func 7)
  (id_1 id_for_func (if 0 then 15 else 8))
)"
